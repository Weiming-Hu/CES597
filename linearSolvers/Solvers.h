/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   Solvers.h
 * Author: Weiming Hu
 *
 * Created on September 10, 2018, 1:10 PM
 */

#ifndef SOLVERS_H
#define SOLVERS_H

#include "Matrix.h"
#include <string>

class Solvers {
public:
    Solvers();
    Solvers(const Solvers& orig) = delete;
    virtual ~Solvers();

    // Compute the inverse of a matrix using the Gaussian Elimination method
    bool inverse(const Matrix & mat, Matrix & mat_inv) const;

    // Multiple two matrices
    bool multiply(const Matrix & mat_lhs, const Matrix & mat_rhs,
            Matrix & mat_mul) const;

    // Transpose a matrix
    bool transpose(const Matrix & mat, Matrix & mat_out) const;

    // Read a matrix from a csv file. The file should only contain numbers and
    // commas. No spaces and headers are permitted.
    //
    bool readMatrix(const std::string & csv_file, Matrix & mat_out) const;
    
    // Read a vector from a csv file. The file should only have one line with
    // only commas and numbers without spaces.
    //
    bool readVector(const std::string & csv_file,
            std::vector<double> & vec_out) const;

    //    bool elimination(Matrix & mat, bool forward) const;
    //    std::vector<double> substitution(const Matrix & lhs_mat,
    //            const std::vector<double> & rhs_vec, bool forward);

private:

};

#endif /* SOLVERS_H */