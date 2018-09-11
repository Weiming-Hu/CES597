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
    size_t nsize = 2;
    Matrix mat(nsize);
    
    mat[0][0] = 4;
    mat[0][1] = 2;
    mat[1][0] = 3;
    mat[1][1] = 4;
    
    
    return 0;
}
