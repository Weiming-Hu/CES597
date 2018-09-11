/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   directSolver.cpp
 * Author: Weiming Hu
 *
 * Created on September 11, 2018, 4:31 PM
 */

#include "Solvers.h"

using namespace std;

int main(int argc, char** argv) {

    if (argc != 3) {
        cout << "directSolvers <matrix csv> <vector csv>" << endl;
        return 0;
    }

    Matrix mat_ori, vec;
    Solvers so;

    so.readMatrix(argv[argc - 2], mat_ori);
    so.readMatrix(argv[argc - 1], vec);
    
    // Check the dimensions of input
    if (mat_ori.nrows() != vec.size()) 
        throw runtime_error("Matrix and vector do not have correct shapes.");
    
    // Solve the system with normal equation
    //       t     t -1
    //  x = A  (A A )   b
    //
    Matrix mat_t, mat_inv, mat_mul, mat_out;
    so.transpose(mat_ori, mat_t);
    so.multiply(mat_ori, mat_t, mat_mul);
    so.inverse(mat_mul, mat_inv);
    so.multiply(mat_t, mat_inv, mat_mul);
    so.multiply(mat_mul, vec, mat_out);
    
    cout << "The solution is " << endl << mat_out << endl;

    return 0;
}
