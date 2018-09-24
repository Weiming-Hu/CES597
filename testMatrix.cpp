/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   testMatrix.cpp
 * Author: Weiming Hu
 *
 * Created on September 10, 2018, 1:31 PM
 */

#include "Matrix.h"

using namespace std;

int main() {
    
    Matrix mat(10);
    
    for (size_t i = 0; i < 10; i++) {
        mat[i][i] = rand() % 1000 + 1;
    }
    cout << mat << endl;
    
    return 0;
}
