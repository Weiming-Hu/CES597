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

class Solvers {
public:
    Solvers();
    Solvers(const Solvers& orig) = delete;
    virtual ~Solvers();

    bool forwardElimination(Matrix & mat) const;
    std::vector<double> solveSubstitution(const Matrix & lhs_mat,
            const std::vector<double> & rhs_vec, bool forward = false);

private:

};

#endif /* SOLVERS_H */