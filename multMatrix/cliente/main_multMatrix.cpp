// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS  2023/2024

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "multmatrix.h"
#include "clientMatrix.h"
#include "operacionesMatrix.h"

void freeMatrix(matrix_t* m);	// liberar la memoria de una matriz
void printMatrix(matrix_t *m);  // imprimir por pantalla una matriz

int main()
{

	clientMatrix* matrix = new clientMatrix();

	if(!matrix->running)
	{
		return 0;
	}
	
	// operaciones
	matrix_t* m1 = matrix->createRandMatrix(5,5);
	matrix_t* m2 = matrix->createIdentity(5,5);
    	matrix_t* mres = matrix->multMatrices(m1,m2);
	matrix->writeMatrix(mres, "resultado.txt");
    	matrix_t* m3 = matrix->readMatrix("resultado.txt");
    	matrix_t* mres2 = matrix->multMatrices(m1,m3);
    	matrix->writeMatrix(mres2, "resultado2.txt");
	
	// liberar memoria
    	freeMatrix(m1);
    	freeMatrix(m2);
    	freeMatrix(mres);
    	freeMatrix(m3);
    	freeMatrix(mres2);
    	delete matrix;
}

void freeMatrix(matrix_t* m){
    	delete[] m->data;
	delete[] m;
}


