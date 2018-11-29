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
#include <algorithm>
#include <iterator>

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <netcdf.h>
#include <netcdf_par.h>

using namespace std;

#define ERR {if(res!=NC_NOERR) {cout << "Error at line=" << __LINE__ << ": "<< nc_strerror(res) << endl;MPI_Finalize();return res;}}

#define NDIMS 2
#define SMALLVAL 1.0e-3

int main(int argc, char **argv) {
    
    // Initialize the MPI world
    int world_size = -1, world_rank = -1;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Parse arguments
    string nc_file;
    double resid = 999;
    size_t start[NDIMS], count[NDIMS];
    int initialize_method = 1, max_it = 10,
            master_rank = 0, opt = -1, verbose = 1;
    
    if (argc >= 4) {
        nc_file = argv[1];
        max_it = atoi(argv[2]);
        initialize_method = atoi(argv[3]);
        
        if (argc == 5) {
           verbose = atoi(argv[4]); 
        }
        
    } else {
        if (world_rank == 0) {
            cout << "parallelJacobi_v2 <netcdf file> <max iteration> "
                    << "<initialization> [verbose level]" << endl;
        }
        return 0;
    }

    // Read NetCDF file meta data
    int ncid = -1, dimid = -1, varid = -1, res = -1;
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

    // Master process should know about how each process is reading data
    int *recvcounts = nullptr, *displs = nullptr;
    if (world_rank == master_rank) {
        recvcounts = new int[world_size]();
        displs = new int[world_size]();

        for (size_t i = 0; i < world_size; i++) {
            recvcounts[i] = i == world_size - 1 ? size - (i * size / world_size) : size / world_size;
            displs[i] = i * size / world_size;
        }
    }

    // Always read the full columns
    start[0] = 0;
    count[0] = size;

    // Read the sampled data
    ptrdiff_t stride[2] = {1, 1};
    double *pA = new double[count[0] * count[1]]();
    double *pb = new double[count[1]]();
    double *px_correct;

    res = nc_inq_varid(ncid, "A", &varid); ERR;
    res = nc_get_vars_double(ncid, varid, start, count, stride, pA); ERR;

    res = nc_inq_varid(ncid, "b", &varid); ERR;
    res = nc_get_vars_double(ncid, varid, start + 1, count + 1, stride + 1, pb); ERR;


    if (world_rank == master_rank) {
        px_correct = new double[size]();
        res = nc_inq_varid(ncid, "x", &varid); ERR;
        res = nc_get_var_double(ncid, varid, px_correct); ERR;
    }

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
    for (size_t i = 0; i < count[1]; i++) {
        index = (i + start[1]) * count[1] + i;
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

        MPI_Bcast(px, size, MPI_DOUBLE, master_rank, MPI_COMM_WORLD);

        // Each process carries out their own computation
        for (size_t i = 0; i < count[1]; i++) {
            for (size_t j = 0; j < count[0]; j++) {
                ptmp[i] += pA[j * count[1] + i] * px[j];
            }

            ptmp[i] = D_inv[i] * (pb[i] - ptmp[i]);
        }

        MPI_Gatherv(ptmp, count[1], MPI_DOUBLE, pdeltax, recvcounts, displs,
                MPI_DOUBLE, master_rank, MPI_COMM_WORLD);

        if (world_rank == master_rank) {
            // Print the sum of residuals
            resid = accumulate(pdeltax, pdeltax + size, 0.0, [](
                    const double lhs, const double rhs) {
                return (lhs + abs(rhs));
            });
            
            if (verbose >= 2)
                cout << "Iteration # " << i_it + 1 << " residual: " << resid << endl;

            // Calculate the new solution
            transform(px, px + size, pdeltax, px, plus<double>());
        }

        MPI_Bcast(&resid, 1, MPI_DOUBLE, master_rank, MPI_COMM_WORLD);
        
    }

    if (world_rank == master_rank && verbose) {
        cout << "The absolute error sum to the answer is ";
        double dif = 0;
        for (size_t i = 0; i < size; i++) {
            dif += abs(px_correct[i] - px[i]);
        }
        cout << dif << "." << endl;
    }

    delete [] pA, pb, px, ptmp, D_inv;
    if (world_rank == master_rank) delete [] pdeltax, px_correct, recvcounts, displs;

    // Housekeeping
    MPI_Finalize();

    return 0;
}
