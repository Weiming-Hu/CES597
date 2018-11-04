/* Copyright (c) 2018 Weiming Hu
 *
 * File:   Matrix.h
 * Author: Weiming Hu
 *
 * Created on September 10, 2018, 1:08 PM
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <exception>

struct continuousMatrix {
    int nrows;
    int ncols;
    int length;
    double *data;
};

void deleteContinuousMatrix(struct continuousMatrix * p_cm);

class Matrix : public std::vector< std::vector<double> > {
public:
    Matrix();
    Matrix(std::size_t nsize);
    Matrix(std::size_t nrows, std::size_t ncols);
    Matrix(const Matrix& orig);
    virtual ~Matrix();
    
    void resize(std::size_t nrows, std::size_t ncols);
    size_t nrows() const;
    size_t ncols() const;
    
    // Check whether the matrix is diagonally dominant
    bool checkDominant()const;
    
    // Read matrix from file
    bool readMatrix(const std::string & csv_file);
    
    // Matrix inverse
    Matrix inverse();
    
    // Matrix transpose
    Matrix transpose();

    // Print functions
    void print(std::ostream &) const;
    
    // Convert to a matrix with continuous memory
    struct continuousMatrix *toContinuousMatrix() const;
    
    // Generate from a matrix with continuous memory
    void fromContinuousMatrix(struct continuousMatrix * p_cm);
    
    // Overload operators
    Matrix & operator=(const Matrix & rhs);
    friend std::ostream & operator<<(std::ostream &, const Matrix &);

    inline friend Matrix operator+(const Matrix & lhs, const Matrix & rhs) {
        using namespace std;
        Matrix mat_add(0, 0);

        if (lhs.ncols_ != rhs.ncols_ || lhs.nrows_ != rhs.nrows_) {
            throw runtime_error("Matrices do not have the correct shape.");
        }

        mat_add.resize(lhs.nrows_, lhs.ncols_);

#if defined(_OPENMP)
#pragma omp parallel for default(none) schedule(static) collapse(2) \
        shared(lhs, mat_add, rhs) 
#endif
        for (size_t i = 0; i < lhs.nrows_; i++) {
            for (size_t j = 0; j < lhs.ncols_; j++) {
                mat_add[i][j] = lhs[i][j] + rhs[i][j];
            }
        }

        return (mat_add);
    }

    inline friend Matrix operator-(const Matrix & lhs, const Matrix & rhs) {
        using namespace std;
        Matrix mat_minus(0, 0);

        if (lhs.ncols_ != rhs.ncols_ || lhs.nrows_ != rhs.nrows_) {
            throw runtime_error("Matrices do not have the correct shape.");
        }

        mat_minus.resize(lhs.nrows_, lhs.ncols_);

#if defined(_OPENMP)
#pragma omp parallel for default(none) schedule(static) collapse(2) \
        shared(lhs, rhs, mat_minus) 
#endif
        for (size_t i = 0; i < lhs.nrows_; i++) {
            for (size_t j = 0; j < lhs.ncols_; j++) {
                mat_minus[i][j] = lhs[i][j] - rhs[i][j];
            }
        }

        return (mat_minus);
    }

    inline friend Matrix operator*(const Matrix & lhs, const Matrix & rhs) {
        using namespace std;
        Matrix mat_mul(0, 0);

        if (lhs.ncols_ != rhs.nrows_) {
            throw runtime_error("Matrices do not have the correct shape.");
        }

        size_t mid = lhs.ncols_;
        size_t nrows = lhs.nrows_;
        size_t ncols = rhs.ncols_;

        mat_mul.resize(nrows, ncols);

#if defined(_OPENMP)
#pragma omp parallel for default(none) schedule(static) collapse(2) \
        shared(nrows, ncols, mat_mul, lhs, rhs, mid)
#endif
        for (size_t i = 0; i < nrows; i++) {
            for (size_t j = 0; j < ncols; j++) {
                double sum = 0.0;
                for (size_t k = 0; k < mid; k++) {
                    sum += lhs[i][k] * rhs[k][j];
                }
                mat_mul[i][j] = sum;
            }
        }

        return (mat_mul);
    }

private:
    std::size_t nrows_ = 0;
    std::size_t ncols_ = 0;
};

#endif /* MATRIX_H */

