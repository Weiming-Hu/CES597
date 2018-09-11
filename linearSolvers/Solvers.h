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

    bool elimination(Matrix & mat, bool forward) const;
    std::vector<double> substitution(const Matrix & lhs_mat,
            const std::vector<double> & rhs_vec, bool forward);
    bool decomposition(const Matrix & mat);
    
    std::vector<double> serialSolver(const Matrix & lhs_mat,
            const std::vector<double> & rhs_vec);

private:

};

#endif /* SOLVERS_H */