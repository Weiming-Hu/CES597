/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   parallelJacobi_v2.cpp
 * Author: Weiming Hu
 *
 * Created on November 26, 2018, 5:38 PM
 */
#include <string>

#include <mpi.h>
#include <stdio.h>
#include <netcdf.h>
#include <netcdf_par.h>

using namespace std;

#define ERR {if(res!=NC_NOERR) cout << "Error at line=" << __LINE__ << ": " << nc_strerror(res) << endl;}

#define NDIMS 2
#define DIMSIZE 24

int main(int argc, char **argv) {

    string nc_file = "/home/graduate/wuh20/github/LinearSystemSolvers/data/ncdf4/100.nc";
    size_t start[NDIMS], count[NDIMS];

    // Initialize the MPI world
    int world_size = -1, world_rank = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Read NetCDF file meta data
    int ncid = -1, dimid = -1, res = -1;
    MPI_Info info;
    res = nc_open_par(nc_file.c_str(), NC_NOWRITE, MPI_COMM_WORLD, MPI_INFO_NULL, &ncid); ERR;

    // Query size from the nc file
    size_t size = 0;
    res = nc_inq_dimid(ncid, "size", &dimid); ERR;
    res = nc_inq_dimlen(ncid, dimid, &size); ERR;

    // Define the rows to read for this process
    start[0] = world_rank * size / world_size;
    count[0] = world_rank == world_size - 1 ? size - (world_rank * size / world_size) : size / world_size;

    // Always read the full colomns
    start[1] = 0; count[1] = size;

    res = nc_read

    res = nc_close(ncid); ERR;
    
//
//    /* Create a parallel netcdf-4 file. */
//    if ((res = nc_create_par(FILE, NC_NETCDF4 | NC_MPIIO, comm,
//            info, &ncid)))
//        BAIL(res);
//
//    /* Create two dimensions. */
//    if ((res = nc_def_dim(ncid, "d1", DIMSIZE, dimids)))
//        BAIL(res);
//    if ((res = nc_def_dim(ncid, "d2", DIMSIZE, &dimids[1])))
//        BAIL(res);
//
//    /* Create one var. */
//    if ((res = nc_def_var(ncid, "v1", NC_INT, NDIMS, dimids, &v1id)))
//        BAIL(res);
//
//    if ((res = nc_enddef(ncid)))
//        BAIL(res);
//
//    /* Set up slab for this process. */
//    start[0] = mpi_rank * DIMSIZE / mpi_size;
//    start[1] = 0;
//    count[0] = DIMSIZE / mpi_size;
//    count[1] = DIMSIZE;
//    printf("mpi_rank=%d start[0]=%d start[1]=%d count[0]=%d count[1]=%d\n",
//            mpi_rank, start[0], start[1], count[0], count[1]);
//
//    /* Create phony data. We're going to write a 24x24 array of ints,
//       in 4 sets of 144. */
//    printf("mpi_rank*QTR_DATA=%d (mpi_rank+1)*QTR_DATA-1=%d\n",
//            mpi_rank*QTR_DATA, (mpi_rank + 1) * QTR_DATA);
//    for (i = mpi_rank * QTR_DATA; i < (mpi_rank + 1) * QTR_DATA; i++)
//        data[i] = mpi_rank;
//
//    /*if ((res = nc_var_par_access(ncid, v1id, NC_COLLECTIVE)))
//      BAIL(res);*/
//    if ((res = nc_var_par_access(ncid, v1id, NC_INDEPENDENT)))
//        BAIL(res);
//
//    /* Write slabs of phony data. */
//    if ((res = nc_put_vara_int(ncid, v1id, start, count,
//            &data[mpi_rank * QTR_DATA])))
//        BAIL(res);
//
//    /* Close the netcdf file. */
//    if ((res = nc_close(ncid)))
//        BAIL(res);

    /* Shut down MPI. */
    MPI_Finalize();

    return 0;
}
