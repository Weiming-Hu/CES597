/* Copyright (c) 2018 Weiming Hu
 *
 * testSolvers.cpp
 *
 * Test the class Solvers.
 */

#include <iterator>

#include "Functions.h"

using namespace std;

int main(int argc, char** argv) {
    
    Functions funcs;
    Matrix mat;
    
    if (argc != 3) {
        cout << "testSolvers <matrix csv> <vector csv>" << endl;
        return 0;
    }
    
    funcs.readMatrix(argv[argc-2], mat);
    cout << "Original matrix: " << endl << mat;
    
    Matrix mat_inv;
    funcs.inverse(mat, mat_inv);
    cout << "Inverse matrix:" << endl << mat_inv;
    
    Matrix mat_mul;
    funcs.multiply(mat, mat_inv, mat_mul);
    cout << "mat * mat_inv = " << endl << mat_mul;
    
    Matrix mat_t;
    funcs.transpose(mat, mat_t);
    cout << "Transpose of original matrix:" << endl << mat_t;
    
    vector<double> vec;
    funcs.readVector(argv[argc-1], vec);
    cout << "Input vector: ";
    copy(vec.begin(), vec.end(), ostream_iterator<double>(cout, " "));
    cout << endl;
    
    return 0;
}
