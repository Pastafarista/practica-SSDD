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
	std::cout << "Main - Operación constructor\n";

	clientMatrix* matrix = new clientMatrix();

	if(!matrix->running)
	{
		return 0;
	}

	std::cout << "Main - Creando matriz aleatoria 5x5\n";
	
	matrix_t* m1 = matrix->createRandMatrix(5,5);
	
	printMatrix(m1);
	
	std::cout << "Main - Creando Matriz identidad 5x5\n";

	matrix_t* m2 = matrix->createIdentity(5,5);
	
	printMatrix(m2);

	std::cout << "Main - Multiplicando ambas matrices (resultado 5x5)\n";

    	matrix_t* mres = matrix->multMatrices(m1,m2);

	printMatrix(mres);

	std::cout << "Main - Escribiendo resultado en servidor\n";

	matrix->writeMatrix(mres, "resultado.txt");

	std::cout << "Main - Leyendo matriz de las multiplicaciones\n";

    	matrix_t* m3 = matrix->readMatrix("resultado.txt");

	printMatrix(m3);

	std::cout << "Main - Multiplicando m1 x m3\n";

    	matrix_t* mres2 = matrix->multMatrices(m1,m3);

	printMatrix(mres2);

	std::cout << "Main - Escribiendo resultado en servidor\n"; 

    	matrix->writeMatrix(mres2, "resultado2.txt");

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

void printMatrix(matrix_t* m)
{
	for(int i = 0; i < m->rows; i++)
	{	
		for(int j = 0; j < m->cols; j++)
		{
			std::cout << m->data[i*m->cols + j] << " ";
		}

		std::cout << std::endl;
	}
}
