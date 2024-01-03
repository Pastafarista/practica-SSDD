// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS 2023/2024

#ifndef MULTMATRIX_H
#define MULTMATRIX_H
#include <string>

typedef struct matrix_t
{
    int rows;
    int cols;
    int* data;
}matrix_t;

class multMatrix
{
public:
    multMatrix();
    matrix_t* readMatrix(std::string fileName);
    matrix_t *multMatrices(matrix_t* m1, matrix_t *m2);
    void writeMatrix(matrix_t* m,std::string fileName);
    ~multMatrix();
    matrix_t *createIdentity(int rows, int cols);
    matrix_t *createRandMatrix(int rows, int cols);
};

#endif // MULTMATRIX_H
