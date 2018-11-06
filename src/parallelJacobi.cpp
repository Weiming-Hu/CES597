/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   parallelJacobi.cpp
 * Author: Weiming Hu
 *
 * Created on October 30, 2018, 4:51 PM
 */

#include "Matrix.h"

#include <algorithm>
#include <numeric>
#include <iomanip>
#include <cmath>
#include <ctime>

#include <mpi.h>
#include <iterator>

using namespace std;

#define _SMALL_VALUE 1.0e-3;

void runJacobi(const Matrix & A, const Matrix & b, Matrix & solution,
        size_t max_it, size_t initialize_func, int verbose) {
    // Jacobi Method
    //
    // We have our serial system set up as x_k+1 = D^-1 * (b - R * x_k)
    // If we rearrange the terms, this becomes the following:
    // D * Δx = b - A * x_k, where the error term Δx = x_k+1 - x_k
    //
    // By doing this transformation, we separate the error term which 
    // makes the parallelization easier.
    //

    int world_size = -1, world_rank = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    Matrix D, D_inv, part_error;
    double resid = 999, small_resid = _SMALL_VALUE;
    

    if (world_rank == 0) {
        D.resize(A.nrows(), A.ncols());
        for (size_t i = 0; i < D.nrows(); i++) D[i][i] = A[i][i];

        D_inv = D.inverse();
        
        solution.resize(A.ncols(), 1);

        if (initialize_func == 1) {
            for (size_t i = 0; i < solution.nrows(); i++) {
                solution[i][0] = 1;
            }
        } else if (initialize_func == 2) {
            std::srand(std::time(nullptr));
            for (size_t i = 0; i < solution.nrows(); i++) {
                solution[i][0] = rand();
            }
        } else if (initialize_func == 3) {
            for (size_t i = 0; i < solution.nrows(); i++) {
                solution[i][0] = b[0][0] / A[0][i] / solution.nrows();
            }
        } else {
            throw runtime_error("Error: Unknown initialize_func.");
        }

        if (verbose >= 3) {
            cout << "A is " << A << "b is " << b;
        }

        if (verbose >= 4) {
            cout << "D is " << D << "Initialized solution: "
                    << solution << endl;
        }
    }

    // Collectively declare memory as remotely accessible
    MPI_Win win_A_data, win_A_nrows, win_A_ncols, win_solution_data,
            win_errors_data, win_b_data, win_resid;
    
    struct continuousMatrix *p_cm_A = NULL, *p_cm_b = NULL, 
            *p_cm_errors = NULL, *p_cm_solution = NULL;


    if (world_rank == 0) {
        // Add A to remote memory
        p_cm_A = A.toContinuousMatrix();
        
        MPI_Win_create(p_cm_A->data, p_cm_A->length * sizeof (double), sizeof (double),
                MPI_INFO_NULL, MPI_COMM_WORLD, &win_A_data);
        MPI_Win_create(&(p_cm_A->nrows), sizeof (int), sizeof (int),
                MPI_INFO_NULL, MPI_COMM_WORLD, &win_A_nrows);
        MPI_Win_create(&(p_cm_A->ncols), sizeof (int), sizeof (int),
                MPI_INFO_NULL, MPI_COMM_WORLD, &win_A_ncols);
        
        // Add errors to remote memory
        p_cm_errors = (struct continuousMatrix *) malloc(sizeof(struct continuousMatrix));
        p_cm_errors->ncols = 1;
        p_cm_errors->length = A.ncols();
        p_cm_errors->nrows = p_cm_errors->length;
        p_cm_errors->data = (double *) malloc(p_cm_errors->nrows * sizeof (double));

        MPI_Win_create(p_cm_errors->data, p_cm_errors->length * sizeof(double),
                sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &win_errors_data);
        
        // Add solution to remote memory
        p_cm_solution = solution.toContinuousMatrix();
        
        MPI_Win_create(p_cm_solution->data, p_cm_solution->length * sizeof(double),
                sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &win_solution_data);
        
        // Add b to remote memory
        p_cm_b = b.toContinuousMatrix();

        MPI_Win_create(p_cm_b->data, p_cm_b->length * sizeof(double), sizeof(double),
                MPI_INFO_NULL, MPI_COMM_WORLD, &win_b_data);
        
        // Add residuals to remote memory
        MPI_Win_create(&resid, sizeof(double), sizeof(double),
                MPI_INFO_NULL, MPI_COMM_WORLD, &win_resid);
        
    } else {
        MPI_Win_create(MPI_BOTTOM, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win_A_data);
        MPI_Win_create(MPI_BOTTOM, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win_A_nrows);
        MPI_Win_create(MPI_BOTTOM, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win_A_ncols);
        MPI_Win_create(MPI_BOTTOM, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win_errors_data);
        MPI_Win_create(MPI_BOTTOM, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win_solution_data);
        MPI_Win_create(MPI_BOTTOM, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win_b_data);
        MPI_Win_create(MPI_BOTTOM, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win_resid);
    }

    int nrows = -1, ncols = -1, nrows_to_read = -1, row_start = -1, row_end = -1;

    // Get number of rows from remote memory
    MPI_Win_fence(0, win_A_nrows);
    MPI_Get(&nrows, 1, MPI_INT, 0, 0, 1, MPI_INT, win_A_nrows);
    MPI_Win_fence(0, win_A_nrows);

    // Define rows for each process that should be read
    nrows_to_read = nrows / world_size;
    row_start = world_rank * nrows_to_read;
    row_end = (world_rank + 1) * nrows_to_read - 1;

    if (world_rank == world_size - 1) {
        // The last process deals with the case when the number of rows cannot
        // be divided using the number of processes.
        //
        nrows_to_read = nrows - nrows_to_read * (world_size - 1);
        row_end = row_start + nrows_to_read - 1;
    }

    // Get the number of columns from remote memory
    MPI_Win_fence(0, win_A_ncols);
    MPI_Get(&ncols, 1, MPI_INT, 0, 0, 1, MPI_INT, win_A_ncols);
    MPI_Win_fence(0, win_A_ncols);

    // Convert rows to matrix
    struct continuousMatrix * p_cm_part_A;
    p_cm_part_A = (struct continuousMatrix *) malloc(sizeof (struct continuousMatrix));
    p_cm_part_A->data = (double *) malloc(nrows_to_read * ncols * sizeof (double));
    p_cm_part_A->length = nrows_to_read * ncols;
    p_cm_part_A->ncols = ncols;
    p_cm_part_A->nrows = nrows_to_read;

    // Get the rows of data A
    MPI_Win_fence(0, win_A_data);
    MPI_Get(p_cm_part_A->data, p_cm_part_A->length, MPI_DOUBLE, 0,
            row_start * ncols, p_cm_part_A->length, MPI_DOUBLE, win_A_data);
    MPI_Win_fence(0, win_A_data);

    Matrix part_A;
    part_A.fromContinuousMatrix(p_cm_part_A);
    deleteContinuousMatrix(p_cm_part_A);
    
    // Get the rows of data b
    struct continuousMatrix * p_cm_part_b;
    p_cm_part_b = (struct continuousMatrix *) malloc(sizeof (struct continuousMatrix));
    p_cm_part_b->data = (double *) malloc(nrows_to_read * 1 * sizeof (double));
    p_cm_part_b->length = nrows_to_read * 1;
    p_cm_part_b->ncols = 1;
    p_cm_part_b->nrows = nrows_to_read;
    
    MPI_Win_fence(0, win_b_data);
    MPI_Get(p_cm_part_b->data, p_cm_part_b->length, MPI_DOUBLE, 0,
            row_start * 1, p_cm_part_b->length, MPI_DOUBLE, win_b_data);
    MPI_Win_fence(0, win_b_data);

    Matrix part_b;
    part_b.fromContinuousMatrix(p_cm_part_b);
    deleteContinuousMatrix(p_cm_part_b);
    
    struct continuousMatrix * p_cm_part_error = NULL;
    double * solution_data = (double *) malloc(ncols * sizeof (double));
    
    if (world_rank != 0) solution.resize(ncols, 1);
    
    for (size_t i_it = 0; i_it < max_it && resid > small_resid; i_it++) {
        
        // Update solution
        MPI_Win_fence(0, win_solution_data);
        MPI_Get(solution_data, ncols, MPI_DOUBLE, 0,
                0, ncols, MPI_DOUBLE, win_solution_data);
        MPI_Win_fence(0, win_solution_data);
        for (size_t i = 0; i < ncols; i++) solution[i][0] = solution_data[i];
        
        // Compute errors
        part_error = part_b - part_A * solution;

        // Write errors
        p_cm_part_error = part_error.toContinuousMatrix();
        MPI_Win_fence(0, win_errors_data);
        MPI_Put(p_cm_part_error->data, p_cm_part_error->length, MPI_DOUBLE,
                0, row_start * 1, p_cm_part_error->length, MPI_DOUBLE, win_errors_data);
        MPI_Win_fence(0, win_errors_data);
        deleteContinuousMatrix(p_cm_part_error);

        // Evaluate errors
        MPI_Barrier(MPI_COMM_WORLD);
        if (world_rank == 0) {
            resid = accumulate(p_cm_errors->data, p_cm_errors->data + p_cm_errors->length, 0.0, [](
                    const double lhs, const double rhs) {
                return (lhs + abs(rhs));
            });

            if (verbose >= 2) {
                cout << "Iteration " << i_it + 1 << " residual: " << resid << endl;
            }
        }
        
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Win_fence(0, win_resid);
        MPI_Get(&resid, 1, MPI_DOUBLE, 0, 0, 1, MPI_DOUBLE, win_resid);
        MPI_Win_fence(0, win_resid);

        // Compute new solution
        if (world_rank == 0) {
            Matrix tmp_error;
            tmp_error.fromContinuousMatrix(p_cm_errors);
            solution = D_inv * tmp_error + solution;
            for (size_t i = 0; i < ncols; i++) p_cm_solution->data[i] = solution[i][0];
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }
    

    if (world_rank == 0) {

        if (!A.checkDominant()) {
            cout << "Warning: Input matrix is not diagonally dominant."
                    << " Jacobi Method might not converge." << endl;
        }

        deleteContinuousMatrix(p_cm_A);
        deleteContinuousMatrix(p_cm_b);
        deleteContinuousMatrix(p_cm_errors);
        deleteContinuousMatrix(p_cm_solution);
    }

    free(solution_data);
    MPI_Win_free(&win_A_data);
    MPI_Win_free(&win_A_nrows);
    MPI_Win_free(&win_A_ncols);
    MPI_Win_free(&win_solution_data);
    MPI_Win_free(&win_errors_data);
    MPI_Win_free(&win_b_data);
    MPI_Win_free(&win_resid);

    return;
}

int main(int argc, char** argv) {

    int world_size = -1, world_rank = -1, verbose = 1;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (argc != 5 && argc != 6) {
        if (world_rank == 0) {
            cout << "parallelJacobi <matrix csv> <vector csv> <maximum iteration> <initilization> [A verbose flag integer]"
                    << endl << endl << "\tVerbose level specification: " << endl << "\t\t0 - Quiet" << endl
                    << "\t\t1 - Result only" << endl << "\t\t2 - The above plus iteration information" << endl
                    << "\t\t3 - The above plus input " << endl << "\t\t4 - The above plus transformed matrix" << endl
                    << endl << "\tInitialization specification: " << endl << "\t\t1 - All 1s" << endl
                    << "\t\t2 - Random numbers" << endl << "\t\t3 - Guess" << endl;
        }
        MPI_Finalize();
        return 0;
    }

    // I store the vector b in form of a matrix with only one column.
    Matrix A, b;
    size_t max_it = 1000, initialize_func = 0;
    
    max_it = atoi(argv[3]);

    if (world_rank == 0) {
        // Tasks for rank 0

        // Read verbose flag
        if (argc == 6) {
            verbose = atoi(argv[argc - 1]);
        }

        // Read input files
        A.readMatrix(argv[1]);
        b.readMatrix(argv[2]);
        
        // Read other command line arguments
        initialize_func = atoi(argv[4]);
    }

#ifdef _PROFILE_TIME
    clock_t time_start = clock();
#endif

#ifdef _WALL_TIME
    double wtime_start = MPI_Wtime();
#endif

    // Define solution
    Matrix solution;

    // Read function name
    runJacobi(A, b, solution, max_it, initialize_func, verbose);

#ifdef _WALL_TIME
    double wtime_end = MPI_Wtime();
#endif

#ifdef _PROFILE_TIME
    clock_t time_end = clock();

    if (world_rank == 0) {
        double duration_total = (time_end - time_start) / (double) CLOCKS_PER_SEC;
        cout << setprecision(4) << "Total time for the iterative method: "
                << duration_total << "s" << endl;
    }
#endif

#ifdef _WALL_TIME
    if (world_rank == 0) {
        double wduration_total = wtime_end - wtime_start;
        cout << setprecision(4) << "Total wall time for the iterative method: "
                << wduration_total << "s" << endl;
    }
#endif

    if (verbose >= 1 & world_rank == 0) {
        cout << "Result from Jacobi x is " << endl << solution << endl;
    }

    MPI_Finalize();
    return 0;
}

