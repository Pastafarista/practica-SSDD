// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS  2023/2024

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "multmatrix.h"
#include "clientMatrix.h"
#include "operacionesMatrix.h"

void freeMatrix(matrix_t* m);	// liberar la memoria de una matriz

int main()
{

	std::cout << "Main - Operación constructor\n";

	clientMatrix* mmatrix=new clientMatrix();

	std::cout << "Main - Creando matriz aleatoria 5x5\n";
	
	matrix_t* m1= mmatrix->createRandMatrix(5,5);
	
	std::cout << "Main - Creando Matriz identidad 5x5\n";

	matrix_t* m2= mmatrix->createIdentity(5,5);

	std::cout << "Main - Multiplicando ambas matrices (resultado 5x5)\n";

    	matrix_t* mres=mmatrix->multMatrices(m1,m2);

	std::cout << "Main - Escribiendo resultado en servidor\n";

	mmatrix->writeMatrix(mres,"resultado.txt");

	std::cout << "Main - Leyendo matriz de las multiplicaciones\n";

    	matrix_t* m3=mmatrix->readMatrix("resultado.txt");

	std::cout << "Main - Multiplicando m3 (que es igual a m1) por m1\n";

    	matrix_t* mres2=mmatrix->multMatrices(m1,m3);

	std::cout << "Main - Escribiendo resultado en servidor\n"; 

    	mmatrix->writeMatrix(mres2,"resultado2.txt");

    	freeMatrix(m1);
    	freeMatrix(m2);
    	freeMatrix(mres);
    	freeMatrix(m3);
    	freeMatrix(mres2);
    	delete mmatrix;
}

void freeMatrix(matrix_t* m){
    	delete[] m->data;
	delete[] m;
}


