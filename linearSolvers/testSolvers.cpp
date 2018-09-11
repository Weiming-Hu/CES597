/* Copyright (c) 2018 Weiming Hu
 *
 * testSolvers.cpp
 *
 * Test the class Solvers.
 */

#include <iterator>

#include "Solvers.h"

using namespace std;

int main(int argc, char** argv) {
    
    Solvers so;
    Matrix mat;
    
    if (argc != 3) {
        cout << "testSolvers <matrix csv> <vector csv>" << endl;
        return 0;
    }
    
    so.readMatrix(argv[argc-2], mat);
    cout << "Original matrix: " << endl << mat;
    
    Matrix mat_inv;
    so.inverse(mat, mat_inv);
    cout << "Inverse matrix:" << endl << mat_inv;
    
    Matrix mat_mul;
    so.multiply(mat, mat_inv, mat_mul);
    cout << "mat * mat_inv = " << endl << mat_mul;
    
    Matrix mat_t;
    so.transpose(mat, mat_t);
    cout << "Transpose of original matrix:" << endl << mat_t;
    
    vector<double> vec;
    so.readVector(argv[argc-1], vec);
    cout << "Input vector: ";
    copy(vec.begin(), vec.end(), ostream_iterator<double>(cout, " "));
    cout << endl;
    
    return 0;
}
