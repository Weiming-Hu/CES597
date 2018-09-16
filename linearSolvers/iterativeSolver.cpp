/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   iterativeSolvers.cpp
 * Author: Weiming Hu
 *
 * Created on September 15, 2018, 4:51 PM
 */

#include "Functions.h"

using namespace std;

int main(int argc, char** argv) {

    if (argc != 3) {
        cout << "iterativeSolver <matrix csv> <vector csv>" << endl;
        return 0;
    }

    Matrix mat_ori, vec;
    Functions funcs;

    funcs.readMatrix(argv[argc - 2], mat_ori);
    funcs.readMatrix(argv[argc - 1], vec);

    // Jacobi method
    Matrix K(mat_ori.nrows()), K_inv;

    for (size_t i = 0; i < K.nrows(); i++) {
        K[i][i] = mat_ori[i][i];
    }

    funcs.inverse(K, K_inv);

    Matrix N;
    funcs.minus(K, mat_ori, N);

    // Initialize the residual and the threshold
    double resid = 999, small_resid = 1.0e-3;

    // Define the maximum iteration number
    size_t max_it = 500;

    // Initialize the solution
    Matrix solution, solution_new(mat_ori.ncols(), 1);
    
    // Detect convergence
    Matrix mat_detector;
    funcs.multiply(K_inv, N, mat_detector);
    

    for (size_t i_it = 0; i_it < max_it && resid > small_resid; i_it++) {

        Matrix mat_mul, mat_add;
        solution = solution_new;

        funcs.multiply(N, solution, mat_mul);
        funcs.add(mat_mul, vec, mat_add);
        funcs.multiply(K_inv, mat_add, solution_new);

        resid = 0;
        
        for (size_t i = 0; i < solution.nrows(); i++) {
            resid += abs(solution_new[i][0] - solution[i][0]);
        }
        
        resid /= solution.nrows();
        cout << "Iteration " << i_it + 1 << " residual : " << resid << endl;
    }
    
    funcs.check_zero(solution);

    cout << "The solution is " << endl << solution << endl;

    return 0;
}

