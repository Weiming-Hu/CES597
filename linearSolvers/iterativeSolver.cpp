/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   iterativeSolvers.cpp
 * Author: Weiming Hu
 *
 * Created on September 15, 2018, 4:51 PM
 */

#include "Matrix.h"

#include <numeric>
#include <ctime>

using namespace std;

int main(int argc, char** argv) {

    if (argc != 3 && argc != 4) {
        cout << "iterativeSolver [-v] <matrix csv> <vector csv>" << endl;
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
    
    // Jacobi method
    //
    // For the linear system Ax = b
    // We have A = D + R
    // The iteration scheme is x_k+1 = D^-1 * (b - R * x_k)
    //
    
    // D is the identity matrix
    Matrix D(A.nrows());
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
    double small_resid = 1.0e-3;

    // Define the maximum iteration number
    size_t max_it = 100;

    // Initialize the solution
    Matrix solution, solution_new(b.nrows(), 1);
    std::srand(std::time(nullptr));
    for (size_t i = 0; i < solution_new.nrows(); i++) {
        solution_new[i][0] = 1;
    }

    if (verbose) {
        cout << "A is " << A << "D is " << D << "R is " << R << endl
                << "Initialized solution: " << solution_new << endl;
    }
    
    for (size_t i_it = 0; i_it < max_it && resid_metric > small_resid; i_it++) {
        
        solution = solution_new;
        solution_new = D_inv * (b - R * solution);
        
        resids = A * solution_new - b;
        resid_metric = accumulate(resids.begin(), resids.end(), 0.0, [](
                const double lhs, const vector<double> rhs) {
            return (lhs + std::abs(rhs[0]));
        });
        
        if (verbose) {
//            cout << "Iteration " << i_it + 1 << " residual : " << resid_metric
//                    << endl << "new solution : " << solution_new << endl;
            cout << "Iteration " << i_it + 1 << " residual : " << resid_metric << endl;
        }
    }

    if (verbose) {
        cout << "Result x is " << endl << solution << endl;
    }

    return 0;
}

