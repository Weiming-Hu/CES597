/* Copyright (c) 2018 Weiming Hu
 *
 * testSolvers.cpp
 *
 * Test the class Solvers.
 */


#include "Solvers.h"

using namespace std;

int main() {
    
    Solvers so;
    size_t nsize = 10;
    Matrix mat(nsize);
    
    for (size_t i = 0; i < nsize; i++)
        for (size_t j = 0; j < nsize; j++)
            mat[i][j] = rand() % 1000 + 1;
    
    cout << "Before elimination: " << endl << mat;
    so.forwardElimination(mat);
    cout << "After elimination: " << endl << mat;
    
    return 0;
}
