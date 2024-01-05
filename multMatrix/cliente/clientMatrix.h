// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS 2023/2024

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "utils.h"
#include "multmatrix.h"

class clientMatrix{
private:
	char *ipServer;
	int port;
	connection_t server;

public:
	bool running = false;	// Variable para saber si el cliente se ha conectado correctamente con algún servidor
	
	/* Funciones */

	clientMatrix();     //Constructor de clienteMatrix, realiza la conexión con el  servidor
	bool conectarBroker(char *IP_BROKER, int PUERTO_BROKER); // Obtiene la ip y el puerto del broker de objetos, devuelve true si hay exito y false en caso contrario
    	~clientMatrix();   //Destructor de clienteMatrix, desconexión con el servidor        
    	matrix_t* readMatrix(std::string fileName);             //Función que lee una matriz del servidor
    	matrix_t *multMatrices(matrix_t* m1, matrix_t *m2);     //Función que envia dos matrices al servidor, y recibe su multiplicación
    	void writeMatrix(matrix_t* m, std::string fileName);    //Manda una matriz al servidor para que la escriba
    	matrix_t *createIdentity(int rows, int cols);           //Envia al servidor el tamaño de una matriz, y recibe de este una matriz identidad
    	matrix_t *createRandMatrix(int rows, int cols);         //Envia al servidor el tamaño de una matriz, y recibe de este una matriz aleatoria
};
