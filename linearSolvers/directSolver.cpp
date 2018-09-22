/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   directSolver.cpp
 * Author: Weiming Hu
 *
 * Created on September 11, 2018, 4:31 PM
 */

#include "Matrix.h"

#include <iomanip>

#ifdef _PROFILE_TIME
#include <ctime>
#endif

using namespace std;

int main(int argc, char** argv) {
    
    if (argc != 3 && argc != 4) {
        cout << "directSolvers <matrix csv> <vector csv> [A verbose flag integer]" 
             << endl << endl << "\tVerbose level specification: " << endl << "\t\t0 - Quiet" << endl
             << "\t\t1 - Result only" << endl << "\t\t2 - The about plus input" << endl;
        return 0; 
    }

    // Read verbose flag
    int verbose = 1;
    if (argc == 4) {
        verbose = atoi(argv[argc - 1]);
    }
    
    // I store the vector b in form of a matrix with only one column.
    Matrix A, b;
    
    // Read input files
    A.readMatrix(argv[1]);
    b.readMatrix(argv[2]);
    
    // Check the dimensions of input
    if (A.nrows() != b.nrows()) 
        throw runtime_error("Matrix and vector do not have correct shapes.");
    
    if (verbose >= 2) cout << "Input matrix A: " << A
            << "Input vector b: " << b;

#ifdef _PROFILE_TIME
    clock_t time_start = clock();
#endif
    
    Matrix A_t(A);
    A_t.transpose();
    
    // Solve the system with normal equation
    //       t     t -1
    //  x = A  (A A )   b
    //
    Matrix x = A_t * (A * A_t).inverse() * b;

#ifdef _PROFILE_TIME
    clock_t time_end = clock();

    double duration_total = (time_end - time_start) / (double) CLOCKS_PER_SEC;
    cout << setprecision(4) << "Total time for the direct method: " << duration_total << "s" << endl;
#endif
    
    if (verbose >= 1) cout << "Result x is: " << x << endl;
    
    return 0;
}
