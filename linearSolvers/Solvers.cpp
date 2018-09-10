/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   Solvers.cpp
 * Author: Weiming Hu
 * 
 * Created on September 10, 2018, 1:10 PM
 */

#include "Solvers.h"
#include "Matrix.h"
#include <exception>

using namespace std;

Solvers::Solvers() {
}

//Solvers::Solvers(const Solvers& orig) {
//}

Solvers::~Solvers() {
}

bool
Solvers::forwardElimination(Matrix & mat) const {
    
    auto nsize = mat.nrows();
    if (nsize != mat.ncols())
        throw runtime_error("Matrix should be square!");
    
    for (size_t k = 0; k < nsize - 1; k++) {
        for (size_t i = k + 1; i < nsize; i++) {
            
            mat[i][k] /= mat[k][k];
            
            for (size_t j = k + 1; j < nsize; j++) {
                mat[i][j] = mat[i][j] - mat[i][k] * mat[k][j];
            }
        }
    }

    return true;
}

std::vector<double>
Solvers::solveSubstitution(const Matrix & lhs_mat,
        const std::vector<double> & rhs_vec, bool forward) {

}