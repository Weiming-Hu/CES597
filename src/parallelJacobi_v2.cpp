/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   parallelJacobi_v2.cpp
 * Author: Weiming Hu
 *
 * Created on November 26, 2018, 5:38 PM
 */
#include <string>
#include <ctime>
#include <numeric>

#include <mpi.h>
#include <stdio.h>
#include <netcdf.h>
#include <netcdf_par.h>
#include <algorithm>
#include <iterator>

using namespace std;

#define ERR {if(res!=NC_NOERR) {cout << "Error at line=" << __LINE__ << ": "<< nc_strerror(res) << endl;MPI_Finalize();}}

#define NDIMS 2
#define DIMSIZE 24
#define SMALLVAL 1.0e-3

int main(int argc, char **argv) {

    string nc_file = "/home/graduate/wuh20/github/LinearSystemSolvers/data/ncdf4/10.nc";
    int initialize_method = 1, max_it = 1000, master_rank = 0;
    double resid = 999;
    
    size_t start[NDIMS], count[NDIMS];

    // Initialize the MPI world
    int world_size = -1, world_rank = -1;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Read NetCDF file meta data
    int ncid = -1, dimid = -1, varid = -1, res = -1;
    MPI_Info info;
    res = nc_open_par(nc_file.c_str(), NC_NOWRITE, MPI_COMM_WORLD, MPI_INFO_NULL, &ncid); ERR;

    // Query size from the nc file
    size_t size = 0;
    res = nc_inq_dimid(ncid, "size", &dimid); ERR;
    res = nc_inq_dimlen(ncid, dimid, &size); ERR;

    // Define the rows to read for this process
    // Note that the first dimension is column and the second dimension is row.
    //
    start[1] = world_rank * size / world_size;
    count[1] = world_rank == world_size - 1 ? size - (world_rank * size / world_size) : size / world_size;

    // Always read the full columns
    start[0] = 0; count[0] = size;

    // Read the sampled data
    ptrdiff_t stride[2] = {1, 1};
    double *pA = new double[count[0] * count[1]]();
    double *pb = new double[count[1]]();
    double *px_correct;
    
    res = nc_inq_varid(ncid, "A", &varid); ERR;
    res = nc_get_vars_double(ncid, varid, start, count, stride, pA); ERR;
    
    res = nc_inq_varid(ncid, "b", &varid); ERR;
    res = nc_get_vars_double(ncid, varid, start+1, count+1, stride+1, pb); ERR;
    
    
    if (world_rank == master_rank) {
        px_correct = new double[size]();
        res = nc_inq_varid(ncid, "x", &varid); ERR;
        res = nc_get_var_double(ncid, varid, px_correct); ERR;
    }

//    // Debugging purpose
//    for (size_t j = 0; j < world_size; j++) {
//        MPI_Barrier(MPI_COMM_WORLD);
//        if (world_rank == j) {
//            cout << "Proc " << world_rank << " A: ";
//            for (size_t i = 0; i < count[0] * count[1]; i++) cout << pA[i] << " ";
//            cout << endl;
//            cout << "Proc " << world_rank << " b: ";
//            for (size_t i = 0; i < count[1]; i++) cout << pb[i] << " ";
//            cout << endl;
//        }
//    }

    res = nc_close(ncid); ERR;
    
    // Initialize the solution x
    double *px = new double[size]();
    
    if (world_rank == master_rank) {
        // Only the first process initialize the solution.
        // This solution will be broadcasted to the other processes later.
        //
        if (initialize_method == 1) {
            for (size_t i = 0; i < size; i++) {
                px[i] = 1;
            }
        } else if (initialize_method == 2) {
            std::srand(std::time(nullptr));
            for (size_t i = 0; i < size; i++) {
                px[i] = rand();
            }
        } else if (initialize_method == 3) {
            for (size_t i = 0; i < size; i++) {
                px[i] = pb[0] / pA[i * count[1]] / size;
            }
        } else {
            MPI_Finalize();
            throw runtime_error("Error: Unknown initialize method.");
        }
    }
    
    // Calculate the inverse of the diagonal matrix of A
    double *D_inv = new double[count[1]]();
    size_t index = 0;
    for (size_t i = start[1]; i < count[1]; i++) {
        index = i * count[1] + i;
        if (pA[index] >= SMALLVAL) {
            D_inv[i] = 1 / pA[index];
        } else {
            throw runtime_error("Error: The inverse diagonal matrix of A cannot be found.");
        }
    }

    // Jacobi Method
    //
    // We have our serial system set up as x_k+1 = D^-1 * (b - R * x_k)
    // If we rearrange the terms, this becomes the following:
    // Δx = D^-1 * (b - A * x_k), where the error term Δx = x_k+1 - x_k
    //
    // By doing this transformation, we separate the error term which 
    // makes the parallelization easier.
    //
    double *ptmp = new double[count[1]]();
    double *pdeltax = nullptr;
    if (world_rank == master_rank)
        pdeltax = new double[size]();
    
    for (size_t i_it = 0; i_it < max_it && resid > SMALLVAL; i_it++) {
        
        // Populate the solution from the master process
        MPI_Bcast(px, size, MPI_DOUBLE, master_rank, MPI_COMM_WORLD);
        
        // Each process carries out their own computation
        for (size_t i = 0; i < count[1]; i++) {
            for (size_t j = 0; j < count[0]; j++) {
                ptmp[i] += pA[j * count[1] + i] * px[j];
            }
            
            ptmp[i] = D_inv[i] * (pb[i] - ptmp[i]);
        }

        // Debugging purpose
        for (size_t j = 0; j < world_size; j++) {
            if (world_rank == j) {
                cout << "Proc " << world_rank << " ptmp: ";
                for (size_t i = 0; i < count[1]; i++) cout << ptmp[i] << " ";
                cout << endl;
            }
            MPI_Barrier(MPI_COMM_WORLD);
        }
        
        // Collect partial solution to the master processor
//        if (world_rank == master_rank) {
//            copy(pdeltax, pdeltax + size, ostream_iterator<double>(cout, " "));
//            cout << endl;
//        }
        MPI_Gatherv(ptmp, count[1], MPI_DOUBLE, pdeltax, (int*) count, (int*) start,
                MPI_DOUBLE, master_rank, MPI_COMM_WORLD);
//        MPI_Gather(ptmp, count[1], MPI_DOUBLE, pdeltax, count[1], MPI_DOUBLE,
//                master_rank, MPI_COMM_WORLD);
//        if (world_rank == master_rank) {
//            copy(pdeltax, pdeltax + size, ostream_iterator<double>(cout, " "));
//            cout << endl;
//        }
        
        if (world_rank == master_rank) {
            // Print the sum of residuals
            resid = accumulate(pdeltax, pdeltax + size, 0.0, [](
                    const double lhs, const double rhs) {
                return (lhs + abs(rhs));
            });
            cout << "Rank # " << world_rank << " iteration # " << i_it + 1
                    << " residual: " << resid << endl;
            
            // Calculate the new solution
            transform(px, px + size, pdeltax, px, plus<double>());
        }
    }
    
    if (world_rank == master_rank) {
        cout << "Results from the Parallel Jacobi Method Version 2:" << endl;
        copy(px, px + size, ostream_iterator<double>(cout, " "));
        cout << endl << "Results from the NetCDFf file:" << endl;
        copy(px_correct, px_correct + size, ostream_iterator<double>(cout, " "));
        cout << endl << "The sum of absolute differences is ";
        double dif = 0;
        for (size_t i = 0; i < size; i++) {
            dif += abs(px_correct[i] - px[i]);
        }
        cout << dif << "." << endl;
    }

    delete [] pA, pb, px, ptmp, D_inv;
    if (world_rank == master_rank)
        delete [] pdeltax, px_correct;
    
    // Housekeeping
    MPI_Finalize();

    return 0;
}
