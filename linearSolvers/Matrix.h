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

class Matrix : public std::vector< std::vector<double> > {
public:
    Matrix();
    Matrix(std::size_t nsize);
    Matrix(std::size_t nrows, std::size_t ncols);
    Matrix(const Matrix& orig) = delete;
    virtual ~Matrix();
    
    void resize(std::size_t nrows, std::size_t ncols);;;
    size_t nrows() const;
    size_t ncols() const;
    
    
    // Print functions
    void print(std::ostream &) const;
    friend std::ostream & operator<<(std::ostream &, const Matrix &);
    
private:
    std::size_t nrows_ = 0;
    std::size_t ncols_ = 0;
};

#endif /* MATRIX_H */

