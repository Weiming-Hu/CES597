/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   directSolver.cpp
 * Author: Weiming Hu
 *
 * Created on September 11, 2018, 4:31 PM
 */

#include "Matrix.h"

using namespace std;

int main(int argc, char** argv) {
    
    if (argc != 3 && argc != 4) {
        cout << "directSolvers [-v] <matrix csv> <vector csv>" << endl;
        return 0;
    }

    // Read verbose flag
    bool verbose = false;
    if (argc == 4) {
        string verbose_str = argv[argc - 3];
        if (verbose_str == "-v" || verbose_str == "--verbose")
            verbose = true;
    }
    
    // I store the vector b in form of a matrix with only one column.
    Matrix A, b;
    
    // Read input files
    A.readMatrix(argv[argc - 2]);
    b.readMatrix(argv[argc - 1]);
    
    // Check the dimensions of input
    if (A.nrows() != b.nrows()) 
        throw runtime_error("Matrix and vector do not have correct shapes.");
    
    if (verbose) cout << "Input matrix A: " << A
            << "Input vector b: " << b;
    
    
    Matrix A_t(A);
    A_t.transpose();
    
    // Solve the system with normal equation
    //       t     t -1
    //  x = A  (A A )   b
    //
    Matrix x = A_t * (A * A_t).inverse() * b;
    
    if (verbose) cout << "Result x is: " << x << endl;
    
    return 0;
}