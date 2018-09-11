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
#include <fstream>
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
Solvers::inverse(const Matrix & mat_in, Matrix & mat_inv) const {

    auto nsize = mat_in.nrows();
    if (nsize != mat_in.ncols())
        throw runtime_error("Matrix should be square!");

    Matrix mat(mat_in);

    // Initialize the inverse matrix to an identity matrix
    mat_inv.resize(nsize, nsize);
    for (size_t i = 0; i < nsize; i++) {
        for (size_t j = 0; j < nsize; j++) {
            mat_inv[i][j] = 0;
        }
        mat_inv[i][i] = 1;
    }

    // Use Gaussian Elimination to solve the system
    //
    // Forward elimination
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

            for (size_t j = 0; j < nsize; j++) {
                mat_inv[i][j] -= mat_inv[k][j] * coef;
            }
        }
    }

    // Change the proceeding of each line to 1
    for (size_t i = 0; i < nsize; i++) {

        double coef = mat[i][i];

        for (size_t j = i; j < nsize; j++) {
            mat[i][j] /= coef;
        }

        for (size_t j = 0; j < nsize; j++) {
            mat_inv[i][j] /= coef;
        }
    }

    // Backward elimination
    for (int i = nsize - 2; i >= 0; i--) {
        for (int j = nsize - 1; j > i; j--) {

            for (int m = 0; m < nsize; m++) {
                mat_inv[i][m] -= mat_inv[j][m] * mat[i][j];
            }

            mat[i][j] -= mat[j][j] * mat[i][j];
        }
    }

    return (true);
}

bool
Solvers::multiply(const Matrix & mat_lhs, const Matrix & mat_rhs,
        Matrix & mat_mul) const {

    if (mat_lhs.ncols() != mat_rhs.nrows()) {
        throw runtime_error("Matrices do not have the correct shape.");
    }

    size_t mid = mat_lhs.ncols();
    size_t nrows = mat_lhs.nrows();
    size_t ncols = mat_rhs.ncols();

    mat_mul.resize(nrows, ncols);

    for (size_t i = 0; i < nrows; i++) {
        for (size_t j = 0; j < ncols; j++) {
            double sum = 0;
            for (size_t k = 0; k < mid; k++) {
                sum += mat_lhs[i][k] * mat_rhs[k][j];
            }
            mat_mul[i][j] = sum;
        }
    }

    return (true);
}

bool
Solvers::transpose(const Matrix & mat, Matrix & mat_out) const {

    size_t nrows = mat.ncols();
    size_t ncols = mat.nrows();
    mat_out.resize(nrows, ncols);

    for (size_t i = 0; i < nrows; i++) {
        for (size_t j = 0; j < ncols; j++) {
            mat_out[i][j] = mat[j][i];
        }
    }

    return (true);
}

bool
Solvers::readMatrix(const std::string & csv_file, Matrix & mat_out) const {
    ifstream file;
    file.open(csv_file);

    if (!file.is_open()) {
        throw runtime_error("File can't be opened.");
    }

    char comma;
    string buffer;
    double tmp_val;
    size_t nrows = 0;
    vector<double> tmp_vec;

    while (!file.eof()) {
        getline(file, buffer);

        if (!buffer.empty()) {
            istringstream iss(buffer);
            while (!iss.eof()) {
                iss >> tmp_val;
                tmp_vec.push_back(tmp_val);
                iss >> comma;
            }
            nrows++;
        }
    }
    
    size_t index = 0;
    size_t ncols = tmp_vec.size() / nrows;
    mat_out.resize(nrows, ncols);

    for (size_t i = 0; i < nrows; i++) {
        for (size_t j = 0; j < ncols; j++, index++) {
            mat_out[i][j] = tmp_vec[index];
        }
    }

    return (true);
}

bool
Solvers::readVector(const std::string & csv_file,
        std::vector<double> & vec_out) const {
    ifstream file;
    file.open(csv_file);
    
    if (!file.is_open()) {
        throw runtime_error("File can't be opened!");
    }
    
    char comma;
    string buffer;
    double tmp_val;
    
    vec_out.clear();
    getline(file, buffer);
    if (!buffer.empty()) {
        istringstream iss(buffer);
        while (!iss.eof()) {
            iss >> tmp_val;
            vec_out.push_back(tmp_val);
            iss >> comma;
        }
    }
    
    return (true);
}

//bool
//Solvers::elimination(Matrix & mat, bool forward) const {
//
//    auto nsize = mat.nrows();
//    if (nsize != mat.ncols())
//        throw runtime_error("Matrix should be square!");
//
//    if (forward) {
//
//        for (size_t k = 0; k < nsize - 1; k++) {
//
//            for (size_t i = k + 1; i < nsize; i++) {
//
//                if (std::abs(mat[k][k]) < _ZERO_LIMIT) {
//                    ostringstream message;
//                    message << "0 occurs (" << mat[k][k]
//                            << "). Please use row permutation.";
//                    throw runtime_error(message.str());
//                }
//
//                double coef = mat[i][k] / mat[k][k];
//
//                for (size_t j = k; j < nsize; j++) {
//                    mat[i][j] -= mat[k][j] * coef;
//                }
//            }
//        }
//    } else {
//        throw runtime_error("Backward elimination is not implemented!");
//    }
//
//    return true;
//}
//
//vector<double>
//Solvers::substitution(const Matrix & lhs_mat,
//        const vector<double> & rhs_vec, bool forward) {
//
//    auto nsize = lhs_mat.nrows();
//    if (nsize != lhs_mat.ncols())
//        throw runtime_error("Matrix should be square!");
//    if (nsize != rhs_vec.size()) {
//        throw runtime_error("Shapes of matrix and vector do not match!");
//    }
//
//    vector<double> results(nsize);
//
//    if (forward) {
//
//        for (size_t i = 0; i < nsize; i++) {
//            double sum = 0;
//
//            for (size_t j = 0; j < i; j++) {
//                sum += lhs_mat[i][j] * rhs_vec[j];
//            }
//
//            if (std::abs(lhs_mat[i][i]) < _ZERO_LIMIT) {
//                ostringstream message;
//                message << "0 occurs (" << lhs_mat[i][i] << ").";
//                throw runtime_error(message.str());
//            }
//
//            results[i] = (rhs_vec[i] - sum) / lhs_mat[i][i];
//        }
//
//    } else {
//
//        for (int i = nsize - 1; i >= 0; i--) {
//            double sum = 0;
//
//            for (int j = nsize - 1; j > i; j--) {
//                sum += lhs_mat[i][j] * rhs_vec[j];
//            }
//
//            if (std::abs(lhs_mat[i][i]) < _ZERO_LIMIT) {
//                ostringstream message;
//                message << "0 occurs (" << lhs_mat[i][i] << ").";
//                throw runtime_error(message.str());
//            }
//
//            results[i] = (rhs_vec[i] - sum) / lhs_mat[i][i];
//        }
//    }
//
//    return (results);
//}
