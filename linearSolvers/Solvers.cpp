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
#include <numeric>
#include <algorithm>
#include <sstream>
#include <cmath>

using namespace std;

static const double _ZERO_LIMIT = 1.0e-9;

Solvers::Solvers() {
}

//Solvers::Solvers(const Solvers& orig) {
//}

Solvers::~Solvers() {
}

bool
Solvers::elimination(Matrix & mat, bool forward) const {

    auto nsize = mat.nrows();
    if (nsize != mat.ncols())
        throw runtime_error("Matrix should be square!");

    if (forward) {

        for (size_t k = 0; k < nsize - 1; k++) {

            for (size_t i = k + 1; i < nsize; i++) {

                if (std::abs(mat[k][k]) < _ZERO_LIMIT) {
                    ostringstream message;
                    message << "0 occurs (" << mat[k][k]
                            << "). Please use row permutation.";
                    throw runtime_error(message.str());
                }

                double coef = mat[i][k] / mat[k][k];

                for (size_t j = k; j < nsize; j++) {
                    mat[i][j] -= mat[k][j] * coef;
                }
            }
        }
    } else {
        throw runtime_error("Backward elimination is not implemented!");
    }

    return true;
}

std::vector<double>
Solvers::substitution(const Matrix & lhs_mat,
        const std::vector<double> & rhs_vec, bool forward) {

    auto nsize = lhs_mat.nrows();
    if (nsize != lhs_mat.ncols())
        throw runtime_error("Matrix should be square!");
    if (nsize != rhs_vec.size()) {
        throw runtime_error("Shapes of matrix and vector do not match!");
    }

    vector<double> results(nsize);

    if (forward) {

        for (size_t i = 0; i < nsize; i++) {
            double sum = 0;

            for (size_t j = 0; j < i; j++) {
                sum += lhs_mat[i][j] * rhs_vec[j];
            }

            if (std::abs(lhs_mat[i][i]) < _ZERO_LIMIT) {
                ostringstream message;
                message << "0 occurs (" << lhs_mat[i][i] << ").";
                throw runtime_error(message.str());
            }

            results[i] = (rhs_vec[i] - sum) / lhs_mat[i][i];
        }

    } else {

        for (int i = nsize - 1; i >= 0; i--) {
            double sum = 0;

            for (int j = nsize - 1; j > i; j--) {
                sum += lhs_mat[i][j] * rhs_vec[j];
            }

            if (std::abs(lhs_mat[i][i]) < _ZERO_LIMIT) {
                ostringstream message;
                message << "0 occurs (" << lhs_mat[i][i] << ").";
                throw runtime_error(message.str());
            }

            results[i] = (rhs_vec[i] - sum) / lhs_mat[i][i];
        }
    }

    return (results);
}

bool
Solvers::decomposition(const Matrix & mat) {
    Matrix mat_U(mat), mat_L(mat.ncols());
    elimination(mat_U, true);
    return true;
}


std::vector<double>
Solvers::serialSolver(const Matrix & lhs_mat, 
        const std::vector<double> & rhs_vec) {
    vector<double> tmp;
    return tmp;
}
