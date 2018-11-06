/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   iterativeSolvers.cpp
 * Author: Weiming Hu
 *
 * Created on September 15, 2018, 4:51 PM
 */

#include "Matrix.h"

#include <numeric>
#include <iomanip>
#include <cmath>
#include <ctime>

#ifdef _OPENMP
#ifdef _WALL_TIME
#include <omp.h>
#endif
#endif


using namespace std;

#define _SMALL_VALUE 1.0e-3;

void runGauss(const Matrix & A, const Matrix & b, Matrix & solution,
        size_t max_it, size_t initialize_func, int verbose) {
    // Gauss-Seidel Method
    //
    // For the linear system Ax = b,
    // let L and U be the strict lower and upper triangular portions of A and
    // D be the diagonal matrix, so that A = L + U + D.
    // We then define M = D + L and N = U so that A = M + N.
    // The iteration scheme is x_k+1 = M^-1 * (b - N * x_k);
    //

    // D is the diagonal matrix
    Matrix D(A.nrows(), A.ncols());
    for (size_t i = 0; i < D.nrows(); i++) D[i][i] = A[i][i];

    // L and U is the strict lower and upper part of the matrix A
    Matrix L(A.nrows(), A.ncols()), U(A.nrows());
    for (size_t i = 0; i < A.nrows(); i++) {
        for (size_t j = 0; j < i; j++) {
            L[i][j] = A[i][j];
        }

        for (size_t j = i + 1; j < A.nrows(); j++) {
            U[i][j] = A[i][j];
        }
    }

    // Define M_inv
    Matrix M_inv = (D + L).inverse();

    // Initialize the residual vector
    Matrix resids;

    // Initialize the residual metric
    double resid_metric = 999;

    // Initialize the residual threshold
    double small_resid = _SMALL_VALUE;

    // Initialize the solution
    Matrix solution_new(b.nrows(), 1);
    if (initialize_func == 1) {
        for (size_t i = 0; i < solution_new.nrows(); i++) {
            solution_new[i][0] = 1;
        }
    } else if (initialize_func == 2) {
        std::srand(std::time(nullptr));
        for (size_t i = 0; i < solution_new.nrows(); i++) {
            solution_new[i][0] = rand();
        }
    } else if (initialize_func == 3) {
        for (size_t i = 0; i < solution_new.nrows(); i++) {
            solution_new[i][0] = b[0][0] / A[0][i] / solution_new.nrows();
        }
    } else {
        throw runtime_error("Error: Unknow initialize_func.");
    }

    if (verbose >= 3) {
        cout << "A is " << A << "b is " << b;
    }

    if (verbose >= 4) {
        cout << "D is " << D << "L is " << L << "U is " << U
            << "Initialized solution: " << solution_new << endl;
    }

    for (size_t i_it = 0; i_it < max_it && resid_metric > small_resid; i_it++) {

        solution = solution_new;
        solution_new = M_inv * (b - U * solution);

        resids = A * solution_new - b;
        resid_metric = accumulate(resids.begin(), resids.end(), 0.0, [](
                const double lhs, const vector<double> rhs) {
            return (lhs + abs(rhs[0]));
        });

        if (verbose >= 2) {
            cout << "Iteration " << i_it + 1 << " residual: " << resid_metric << endl;
        }
    }

    if (!A.checkDominant()) {
        cout << "Warning: Input matrix is not diagonally dominant."
                << " Jacobi Method might not converge." << endl;
    }

    if (verbose >= 4) {
        cout << "The iteration matrix M_inv * N is: " << M_inv * U << endl;
    }

    return;
}

void runJacobi(const Matrix & A, const Matrix & b, Matrix & solution,
        size_t max_it, size_t initialize_func, int verbose) {
    // Jacobi Method
    //
    // For the linear system Ax = b,
    // let D be the diagonal matrix and R = A - D, so that A = D + R.
    // The iteration scheme is x_k+1 = D^-1 * (b - R * x_k)
    //

#ifdef _PROFILE_TIME
    clock_t time_start = clock();
#endif

#ifdef _WALL_TIME
    double wtime_start = omp_get_wtime();
#endif

    // D is the diagonal matrix
    Matrix D(A.nrows(), A.ncols());
    for (size_t i = 0; i < D.nrows(); i++) D[i][i] = A[i][i];

    // Inverse matrix D
    Matrix D_inv(D.inverse());

    // Initialize the matrix R
    Matrix R = A - D;

    // Initialize the residual vector
    Matrix resids;

    // Initialize the residual metric
    double resid_metric = 999;

    // Initialize the residual threshold
    double small_resid = _SMALL_VALUE;

    // Initialize the solution
    Matrix solution_new(b.nrows(), 1);
    if (initialize_func == 1) {
        for (size_t i = 0; i < solution_new.nrows(); i++) {
            solution_new[i][0] = 1;
        }
    } else if (initialize_func == 2) {
        std::srand(std::time(nullptr));
        for (size_t i = 0; i < solution_new.nrows(); i++) {
            solution_new[i][0] = rand();
        }
    } else if (initialize_func == 3) {
        for (size_t i = 0; i < solution_new.nrows(); i++) {
            solution_new[i][0] = b[0][0] / A[0][i] / solution_new.nrows();
        }
    } else {
        throw runtime_error("Error: Unknow initialize_func.");
    }

    if (verbose >= 3) {
        cout << "A is " << A << "b is " << b;
    }

    if (verbose >= 4) {
        cout  << "D is " << D << "R is " << R
            << "Initialized solution: " << solution_new << endl;
    }

#ifdef _PROFILE_TIME
    clock_t time_end_of_preprocessing = clock();
#endif

#ifdef _WALL_TIME
    double wtime_end_of_preprocessing = omp_get_wtime();
#endif
    resids.resize(solution_new.nrows(), 1);

#if defined(_OPENMP)
#pragma omp parallel default(none) \
shared(max_it, small_resid, D_inv, b, R, A, verbose, solution_new, resids, solution, resid_metric, cout)
#endif
    {
        int thread_num = omp_get_thread_num();
        for (size_t i_it = 0; i_it < max_it && resid_metric > small_resid; i_it++) {

            if (thread_num == 0) {
                solution = solution_new;
            }

#pragma omp barrier
#pragma omp for schedule(static)
            for (int i_row = 0; i_row < solution_new.nrows(); i_row++) {

                double sum = 0.0;
                for (int i_col = 0; i_col < R.ncols(); i_col++) {
                    sum += R[i_row][i_col] * solution[i_col][0];
                }
                solution_new[i_row][0] = b[i_row][0] - sum;
            }
#pragma omp barrier
#pragma omp for schedule(static)
            for (int i_row = 0; i_row < solution_new.nrows(); i_row++) {
                double sum = 0.0;
                auto solution_old = solution_new;
                for (int i_col = 0; i_col < D_inv.ncols(); i_col++) {
                    sum += D_inv[i_row][i_col] * solution_old[i_col][0];
                }
                solution_new[i_row][0] = sum;
            }
            //solution_new = D_inv * (b - R * solution);
#pragma omp barrier
#pragma omp for schedule(static)
            for (int i_row = 0; i_row < resids.nrows(); i_row++) {
                double sum = 0.0;
                for (int i_col = 0; i_col < R.ncols(); i_col++) {
                    sum += A[i_row][i_col] * solution_new[i_col][0];
                }
                resids[i_row][0] = sum - b[i_row][0];
            }
            //resids = A * solution_new - b;
#pragma omp barrier

            if (thread_num == 0) {
                resid_metric = accumulate(resids.begin(), resids.end(), 0.0, [](
                            const double lhs, const vector<double> rhs) {
                        return (lhs + abs(rhs[0]));
                        });

                if (verbose >= 2) {
                    cout << "Iteration " << i_it + 1 << " residual: " << resid_metric << endl;
                }
            }
        }
    }

#ifdef _PROFILE_TIME
    clock_t time_end_of_loop = clock();
#endif

#ifdef _WALL_TIME
    double wtime_end_of_loop = omp_get_wtime();
#endif

    if (resid_metric > small_resid) {
        cout << " Warning: Jacobi Method did not converge." << endl;

        if (!A.checkDominant()) {
            cout << "Warning: Input matrix is not diagonally dominant.";
        }
    }

    if (verbose >= 4) {
        cout << "The iteration matrix D_inv * R is: " << D_inv * R << endl;
    }
    
#ifdef _WALL_TIME
    double wtime_end = omp_get_wtime();
#endif

#ifdef _PROFILE_TIME
    clock_t time_end = clock();

    double duration_total = (time_end - time_start) / (double) CLOCKS_PER_SEC;
    double duration_preprocessing = (time_end_of_preprocessing - time_start) / (double) CLOCKS_PER_SEC;
    double duration_loop = (time_end_of_loop - time_end_of_preprocessing) / (double) CLOCKS_PER_SEC;
    double duration_postprocessing = (time_end - time_end_of_loop) / (double) CLOCKS_PER_SEC;

    cout << "(Jacobi) Preprocessing: " << duration_preprocessing << "s (" << 100 * duration_preprocessing / duration_total << "%)" << endl
        << "(Jacobi) Loop: " << duration_loop << "s (" << 100 * duration_loop / duration_total << "%)" << endl
        << "(Jacobi) Postprocessing: " << duration_postprocessing << "s (" << 100 * duration_postprocessing / duration_total << "%)" << endl
        << "(Jacobi) Total time: " << duration_total << "s (100%)" << endl;
#endif

#ifdef _WALL_TIME

    double wduration_total = wtime_end - wtime_start;
    double wduration_preprocessing = wtime_end_of_preprocessing - wtime_start;
    double wduration_loop = wtime_end_of_loop - wtime_end_of_preprocessing;
    double wduration_postprocessing = wtime_end - wtime_end_of_loop;

    cout << "(Jacobi) Wall time Preprocessing: " << wduration_preprocessing << "s (" << 100 * wduration_preprocessing / wduration_total << "%)" << endl
        << "(Jacobi) Wall time Loop: " << wduration_loop << "s (" << 100 * wduration_loop / wduration_total << "%)" << endl
        << "(Jacobi) Wall time Postprocessing: " << wduration_postprocessing << "s (" << 100 * wduration_postprocessing / wduration_total << "%)" << endl
        << "(Jacobi) Wall time Total time: " << wduration_total << "s (100%)" << endl;
#endif

    return;
}

int main(int argc, char** argv) {

#ifdef _WALL_TIME
    double wtime_start = omp_get_wtime();
#endif

#ifdef _PROFILE_TIME
    clock_t time_start = clock();
#endif

    if (argc != 6 && argc != 7) {
        cout << "iterativeSolver <Jacobi,J|Gauss,G> <matrix csv> <vector csv> <maximum iteration> <initilization> [A verbose flag integer]"
             << endl << endl << "\tVerbose level specification: " << endl << "\t\t0 - Quiet" << endl
             << "\t\t1 - Result only" << endl << "\t\t2 - The above plus iteration information" << endl
             << "\t\t3 - The above plus input " << endl << "\t\t4 - The above plus transformed matrix" << endl
             << endl << "\tInitialization specification: " << endl << "\t\t1 - All 1s" << endl
             << "\t\t2 - Random numbers" << endl << "\t\t3 - Guess" << endl;
        return 0;
    }

    // Read verbose flag
    int verbose = 1;
    if (argc == 7) {
        verbose = atoi(argv[argc - 1]);
    }

    // I store the vector b in form of a matrix with only one column.
    Matrix A, b;

    // Read input files
    A.readMatrix(argv[2]);
    b.readMatrix(argv[3]);

    size_t max_it = atoi(argv[4]);
    size_t initialize_func = atoi(argv[5]);

#ifdef _WALL_TIME
    double wtime_end_of_read = omp_get_wtime();
#endif

#ifdef _PROFILE_TIME
    clock_t time_end_of_read = clock();
#endif

    // Define solution
    Matrix solution;

    // Read function name
    string function_str(argv[1]);
    if (function_str == "Jacobi" || function_str == "J") {
        runJacobi(A, b, solution, max_it, initialize_func, verbose);

    } else if (function_str == "Gauss" || function_str == "G") {
        runGauss(A, b, solution,  max_it, initialize_func, verbose);

    } else {
        cout << "Error: Unknown function name " << function_str << endl;
        return 1;
    }

    if (verbose >= 1) {
        if (function_str == "Jacobi" || function_str == "J") {
            cout << "Result from Jacobi x is " << endl << solution << endl;
        } else if (function_str == "Gauss" || function_str == "G") {
            cout << "Result from Gauss-Seidel x is " << endl << solution << endl;
        }
    }

#ifdef _WALL_TIME
    double wtime_end = omp_get_wtime();
#endif

#ifdef _PROFILE_TIME
    clock_t time_end = clock();

    double duration_total = (time_end - time_start) / (double) CLOCKS_PER_SEC;
    double duration_reading = (time_end_of_read - time_start) / (double) CLOCKS_PER_SEC;
    double duration_function = (time_end - time_end_of_read) / (double) CLOCKS_PER_SEC;
    cout << "Reading data: " << duration_reading << "s (" << 100 * duration_reading / duration_total << "%)" << endl
        << "Iterative method: " << duration_function << "s (" << 100 * duration_function / duration_total << "%)" << endl
        << "Total time: " << duration_total << "s (100%)" << endl;
#endif

#ifdef _WALL_TIME
    double wduration_total = wtime_end - wtime_start;
    double wduration_reading = wtime_end_of_read - wtime_start;
    double wduration_function = wtime_end - wtime_end_of_read;

    cout << "Wall time Reading data: " << wduration_reading << "s (" << 100 * wduration_reading / wduration_total << "%)" << endl
        << "Wall time Iterative method: " << wduration_function << "s (" << 100 * wduration_function / wduration_total << "%)" << endl
        << "Wall time Total time: " << wduration_total << "s (100%)" << endl;
#endif

    return 0;
}

