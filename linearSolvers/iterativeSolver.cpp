/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   iterativeSolvers.cpp
 * Author: Weiming Hu
 *
 * Created on September 15, 2018, 4:51 PM
 */

using namespace std;

int main(int argc, char** argv) {


    Matrix A, b;
    Functions funcs;
    bool verbose = false;

    if (argc == 3) {
        funcs.readMatrix(argv[argc - 2], A);
        funcs.readMatrix(argv[argc - 1], b);
    } else if (argc == 4) {
        funcs.readMatrix(argv[argc - 3], A);
        funcs.readMatrix(argv[argc - 2], b);
        
        string tmp(argv[argc-1]);
        if (tmp == "-v" || tmp == "--verbose") {
            verbose = true;
        }

    } else {
        cout << "iterativeSolver <matrix csv> <btor csv> [-v,--verbose]" << endl;
        return 0;
    }

    // Jacobi method
    //
    // Target Ax = b
    // We have A = K + N
    // The iteration scheme is x_k+1 = K^-1 * (b - N * x_k)
    //
    Matrix K(A.nrows()), K_inv;

    for (size_t i = 0; i < K.nrows(); i++) {
        K[i][i] = A[i][i];
    }

    funcs.inverse(K, K_inv);

    Matrix N;
    funcs.minus(A, K, N);

    if (verbose) {
        cout << "A is " << A << endl
            << "K is " << K << endl
            << "N is " << N << endl;
    }

    // Initialize the residual and the threshold
    double resid = 999, small_resid = 1.0e-3;

    // Define the maximum iteration number
    size_t max_it = 500;

    // Initialize the solution
    Matrix solution, solution_new(A.ncols(), 1);

    std::srand(std::time(nullptr));
    for (size_t i = 0; i < solution_new.nrows(); i++) {
        solution_new[i][0] = 1;
    }

    if (verbose)
        cout << "Initialized solution: " << endl << solution_new << endl;
    
    // The iteration scheme is x_k+1 = K^-1 * (b - N * x_k)
    for (size_t i_it = 0; i_it < max_it && resid > small_resid; i_it++) {

        Matrix mat_mul, mat_add;
        solution = solution_new;

        funcs.multiply(N, solution, mat_mul);
        funcs.minus(b, mat_mul, mat_add);
        funcs.multiply(K_inv, mat_add, solution_new);

        if (verbose) cout << "new solution : " << solution_new << endl;

        resid = 0;
        for (size_t i = 0; i < solution.nrows(); i++) {
            resid += abs(solution_new[i][0] - solution[i][0]);
        }
        
        resid /= solution.nrows();
        if (verbose) cout << "Iteration " << i_it + 1 << " residual : " << resid << endl;
    }
    
    funcs.check_zero(solution);

    cout << "The solution is " << endl << solution << endl;

    return 0;
}

