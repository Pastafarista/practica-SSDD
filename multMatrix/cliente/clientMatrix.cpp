// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS  2023/2024

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "utils.h"
#include "operacionesMatrix.h"
#include "multmatrix.h"
#include "clientMatrix.h"

clientMatrix::clientMatrix(){

	std::vector<unsigned char> buffOut;
	std::vector<unsigned char> buffIn;

	buffOut.resize(0);
	buffIn.resize(0);
	
		
	std::cout << "Conectando con el servidor " << this->ipServer << ":" << this->port << std::endl;

	this->server = initClient(this->ipServer, this->port);

	if(this->server.socket == -1)
	{
		std::cout << "Error al conectar con el servidor (el sercidor no responde)\n";
		return;
	}
	
	// Enviamos al servidor la operación de constructor
	pack<rpcTipoOperacion>(buffOut, constructorClientOp);

	sendMSG(this->server.serverId, buffOut);
	
	// Recibimos la respuesta del servidor
	recvMSG(this->server.serverId, buffIn);

	int ok = unpack<int>(buffIn);
	if(ok){
		std::cout << "Conexión exitosa con el servidor\n";
	}else{
		std::cout << "Error al iniciar conexión con server\n";
		return;
	}
}

clientMatrix::~clientMatrix(){
	
	std::vector<unsigned char> buffOut;
	std::vector<unsigned char> buffIn;

	buffOut.resize(0);
	buffIn.resize(0);
	
	pack<rpcTipoOperacion>(buffOut, destructorClientOp);
	
	sendMSG(this->server.serverId, buffOut);
	recvMSG(this->server.serverId, buffIn);

	int ok = unpack<int>(buffIn);
	if(ok){
		closeConnection(this->server.serverId);
	}else{
		std::cout << "Error al cerrar conexión con server\n";
	}
}

matrix_t *clientMatrix::readMatrix(std::string fileName){
	std::vector<unsigned char> buffIn;
	std::vector<unsigned char> buffOut;
	buffOut.resize(0);
	buffIn.resize(0);

	//Empaquetamos operación, empaquetamos longitud del nombre del archivo y el nombre
	pack<rpcTipoOperacion>(buffOut,readOp);
	pack(buffOut, (int)fileName.length());
	packv(buffOut, fileName.data(), (int)fileName.length());
	
	//Enviamos operación
	sendMSG(this->server.serverId, buffOut);

	//Recibimos respuesta
	recvMSG(this->server.serverId, buffIn);
	int ok = unpack<int>(buffIn);

	if(ok){
		matrix_t* matrix = new matrix_t[1];
		matrix->rows = unpack<int>(buffIn);
		matrix->cols = unpack<int>(buffIn);
		matrix->data = new int[matrix->rows * matrix->cols];
		unpackv(buffIn, matrix->data, (matrix->rows * matrix->cols));

		return matrix;
	}else{
		std::cout << "Error al leer archivo\n";
		return NULL;
	}
}

matrix_t *clientMatrix::multMatrices(matrix_t *m1, matrix_t* m2){
	std::vector<unsigned char> buffIn;
	std::vector<unsigned char> buffOut;
	buffOut.resize(0);
	buffIn.resize(0);

	//Empaquetamos operación
	pack<rpcTipoOperacion>(buffOut, multOp);

	//Empaquetamos la primera matriz
	pack(buffOut,(int)m1->rows);
	pack(buffOut,(int)m1->cols);
	packv(buffOut, m1->data, (m1->rows * m1->cols));

	//Empaquetamos la segunda matriz
	pack(buffOut,(int)m2->rows);
	pack(buffOut,(int)m2->cols);
	packv(buffOut, m2->data, (m2->rows * m2->cols));

	//Enviamos operación
	sendMSG(this->server.serverId, buffOut);

	//Recibimos respuesta
	recvMSG(this->server.serverId, buffIn);

	int ok = unpack<int>(buffIn);

	if(ok){

		matrix_t* matrix = new matrix_t[1];
		matrix->rows = unpack<int>(buffIn);
		matrix->cols = unpack<int>(buffIn);
		matrix->data = new int[matrix->rows * matrix->cols];
		unpackv(buffIn, matrix->data, (matrix->rows * matrix->cols));

		return matrix;
	}else{
		std::cout << "Error al realizar la multiplicación\n";
		return NULL;
	}
}

void clientMatrix::writeMatrix(matrix_t *m,std::string fileName){

	std::vector<unsigned char> buffIn;
	std::vector<unsigned char> buffOut;
	buffOut.resize(0);
	buffIn.resize(0);

	//Empaquetamos operación
	pack<rpcTipoOperacion>(buffOut,writeOp);
	pack(buffOut,(int)m->rows);
	pack(buffOut,(int)m->cols);
	packv(buffOut, m->data, (m->rows * m->cols));

	pack(buffOut, (int)fileName.length()+1);
	packv(buffOut, fileName.data(), (int)fileName.length()+1);

	//Enviamos operación
	sendMSG(this->server.serverId, buffOut);

	//Recibimos respuesta de confirmación
	recvMSG(this->server.serverId, buffIn);
	int ok = unpack<int>(buffIn);

	if(ok){
		std::cout << "Se ha escrito la matriz en: " << fileName << std::endl;
	}
}

matrix_t *clientMatrix::createIdentity(int rows, int cols){

	std::cout << "Creando matriz identidad " << rows << "x" << cols << std::endl;

	std::vector<unsigned char> buffIn;
	std::vector<unsigned char> buffOut;
	buffOut.resize(0);
	buffIn.resize(0);

	//Empaquetamos el tipo de operación
	pack<rpcTipoOperacion>(buffOut,identityOp);
	pack(buffOut,(int)rows);
	pack(buffOut,(int)cols);

	//Enviamos operación
	sendMSG(this->server.serverId, buffOut);

	//Recibimos respuesta de confirmación
	recvMSG(this->server.serverId, buffIn);
	int ok = unpack<int>(buffIn);

	if(ok){
		std::cout << "Creada matriz identidad\n";
		matrix_t* matrix=new matrix_t[1];
		matrix->rows = unpack<int>(buffIn);
		matrix->cols = unpack<int>(buffIn);
		matrix->data = new int[matrix->rows * matrix->cols];
		unpackv(buffIn, matrix->data, (matrix->rows * matrix->cols));

		return matrix;
	}
}

matrix_t *clientMatrix::createRandMatrix(int rows, int cols){
	
	std::cout << "Creando matriz aleatoria " << rows << "x" << cols << std::endl;

	std::vector<unsigned char> buffIn;
	std::vector<unsigned char> buffOut;
	buffOut.resize(0);
	buffIn.resize(0);

	//Empaquetamos el tipo de operación
	pack<rpcTipoOperacion>(buffOut,randOp);
	pack(buffOut,(int)rows);
	pack(buffOut,(int)cols);
	
	//Enviamos operación
	sendMSG(this->server.serverId, buffOut);

	std::cout << "Esperando respuesta del servidor\n";

	//Recibimos respuesta
	recvMSG(this->server.serverId, buffIn);

	int ok = unpack<int>(buffIn);

	if(ok){
		std::cout << "Creada matriz aleatoria\n";
		matrix_t* matrix = new matrix_t[1];
		matrix->rows = unpack<int>(buffIn);
		matrix->cols = unpack<int>(buffIn);
		matrix->data = new int[matrix->rows * matrix->cols];
		unpackv(buffIn, matrix->data, (matrix->rows * matrix->cols));

		return matrix;
	}
	else{
		std::cout << "Error al crear la matriz aleatoria\n";
		return NULL;
	}
}

