/* Copyright (c) 2018 Weiming Hu
 * 
 * File:   testMatrix.cpp
 * Author: Weiming Hu
 *
 * Created on September 10, 2018, 1:31 PM
 */

#include "Matrix.h"

#include <iterator>

using namespace std;

int main() {
    
    cout << "---------------------" << endl
            << "Test assignment and printing" << endl
            << "---------------------" << endl;
    
    Matrix mat(10);
    
    for (size_t i = 0; i < 10; i++) {
        mat[i][i] = rand() % 1000 + 1;
    }

    cout << mat << endl;

    cout << "---------------------" << endl
            << "Test conversion to/from a Continuous Matrix" << endl
            << "---------------------" << endl;
    
    struct continuousMatrix * cm = mat.toContinuousMatrix();
    
    copy(cm->data, cm->data + cm->length, ostream_iterator<double>(cout, " "));
    
    cout << endl << "Change first and last 5 values to 1 ..." << endl;
    
    cm->data[0] = 1;
    cm->data[1] = 1;
    cm->data[2] = 1;
    cm->data[3] = 1;
    cm->data[4] = 1;
    
    cm->data[cm->length - 1] = 1;
    cm->data[cm->length - 2] = 1;
    cm->data[cm->length - 3] = 1;
    cm->data[cm->length - 4] = 1;
    cm->data[cm->length - 5] = 1;
    
    cout << "Assign the modified continuous matrix back to the original matrix ..." << endl;
    
    mat.fromContinuousMatrix(cm);
    
    cout << "Modified matrix: " << endl << mat << endl;
    
    deleteContinuousMatrix(cm);
    
    return 0;
}
