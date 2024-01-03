// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS  2023/2024

# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <string>
# include "utils.h"
# include "operacionesMatrix.h"
# include "multmatrix.h"
# include "clientMatrix.h"

// constantes
# define PUERTO_BROKER 15015
const char *IP_BROKER = "3.226.246.58";

void printMatrix(matrix_t* m);	// imprimir los contenidos de una matriz

clientMatrix::clientMatrix(){
	
	// declaramos los buffers de entrada y salida
	std::vector<unsigned char> buffOut;
	std::vector<unsigned char> buffIn;
	buffOut.resize(0);
	buffIn.resize(0);
	
	// verbose
	std::cout << "Cliente iniciado\n";
	std::cout << "Conectando con el broker...\n";
	
	// si no conecta con el broker salimos del programa, por lo que running=false y se dejará de ejecutar
	if(!this->conectarBroker())
	{
		return;
	}
		
	// conectar con el servidor que nos ha dado el broker
	std::cout << "Conectando con el servidor " << this->ipServer << ":" << this->port << std::endl;
	this->server = initClient(this->ipServer, this->port);
	
	// comprobar que se ha conectado correctamente
	if(this->server.socket == -1)
	{
		std::cout << "Error al conectar con el servidor (el servidor no responde)\n";
		return;
	}
	
	// Enviamos al servidor la operación de constructor
	pack<rpcTipoOperacion>(buffOut, constructorClientOp);
	sendMSG(this->server.serverId, buffOut);
	
	// Recibimos la respuesta del servidor
	recvMSG(this->server.serverId, buffIn);
	int ok = unpack<int>(buffIn);
	
	// mostrar resultado
	if(ok){
		std::cout << "Conexión exitosa con el servidor\n";
		this->running = true;
	}else{
		std::cout << "Error al iniciar conexión con server\n";
		return;
	}
}

bool clientMatrix::conectarBroker(){

	// conectar con el servidor del broker
	auto serverConnection = initClient(IP_BROKER, PUERTO_BROKER);

    	// comprobar si se ha podido conectar
    	if(serverConnection.socket == -1)
    	{
        	std::cout << "Error al conectar con el broker (servidor del broker no responde)\n";
        	return false;
    	}

    	// declaramos los buffers de entrada y salida
    	std::vector<unsigned char> buffOut;
	std::vector<unsigned char> buffIn;

    	// empaquetar operación
    	pack(buffOut, opConnectClient); // empaquetar tipo de operación
    	pack(buffOut, tipoMultmatrix); // empaquetar tipo de objetos

    	// enviar operación
    	sendMSG(serverConnection.serverId, buffOut);

    	// recibir respuesta
    	recvMSG(serverConnection.serverId, buffIn);
    	int ok = unpack<int>(buffIn);

    	if(ok)
    	{
        	std::cout << "Conexión con el broker establecida\n";

        	// desempaquetar ip
        	std::string ip;
        	int ipLength = unpack<int>(buffIn);
        	ip.resize(ipLength);
        	unpackv(buffIn, (char *)ip.data(), ipLength);
        
        	// desempaquetar puerto
        	int puerto = unpack<int>(buffIn);

        	// guardar datos del servidor de objetos
        	this->ipServer = new char[ipLength];
        	strcpy(this->ipServer, ip.data());
        	this->port = puerto;

        	return true;
    	}
    	else
    	{
        	std::cout << "Error al conectar con el broker (no hay servidores de objetos)\n";
        	return false;
    	}
}

clientMatrix::~clientMatrix(){
	
	std::cout << "Cerrando la conexión con el servidor " << this->ipServer << ":" << this->port << std::endl; 

	// declarar buffers de entrada y salida
	std::vector<unsigned char> buffOut;
	std::vector<unsigned char> buffIn;
	buffOut.resize(0);
	buffIn.resize(0);
	
	// empaquetamos la operación
	pack<rpcTipoOperacion>(buffOut, destructorClientOp);
	
	// enviamos la operación
	sendMSG(this->server.serverId, buffOut);

	// recibimos la operación
	recvMSG(this->server.serverId, buffIn);
	int ok = unpack<int>(buffIn);

	if(ok){
		// si el servidor la valida, cerramos la conexión con el servidor
		closeConnection(this->server.serverId);
		this->running = false;

		// mostrar por pantalla el resultado
		std::cout << "Conexión cerrrada correctamente\n";
	}else{
		std::cout << "Error al cerrar conexión con server\n";
	}
}

matrix_t *clientMatrix::readMatrix(std::string fileName){

	std::cout << "Leyendo la matriz en el archivo " << fileName << std::endl;
	
	// declarar los buffers de entrada y salida
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
		// si el servidor logra leer el archivo desempaquetamos la matriz
		matrix_t* matrix = new matrix_t[1];
		matrix->rows = unpack<int>(buffIn);
		matrix->cols = unpack<int>(buffIn);
		matrix->data = new int[matrix->rows * matrix->cols];
		unpackv(buffIn, matrix->data, (matrix->rows * matrix->cols));
		
		// mostramos el resultado por pantalla
		std::cout << "La matriz de " << fileName << " es:\n";
		printMatrix(matrix);

		return matrix;
	}else{
		// si el servidor no logra leer el archivo devolvemos NULL
		std::cout << "Error al leer archivo\n";
		return NULL;
	}
}

matrix_t *clientMatrix::multMatrices(matrix_t *m1, matrix_t* m2){
	
	// mostrar la operación por pantalla
	std::cout << "Multiplicando:\n";
	printMatrix(m1);
	std::cout << "x\n";
	printMatrix(m2);
	
	// declaramos los buffers de entrada y salida
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
		// si la multiplicación no ha dado errores, desempaquetamos la matriz resultado
		matrix_t* matrix = new matrix_t[1];
		matrix->rows = unpack<int>(buffIn);
		matrix->cols = unpack<int>(buffIn);
		matrix->data = new int[matrix->rows * matrix->cols];
		unpackv(buffIn, matrix->data, (matrix->rows * matrix->cols));

		// mostramos el resultado por mensaje
		std::cout << "El resultado es:\n";
		printMatrix(matrix);

		return matrix;
	}else{
		// si la multiplicación ha dado errores devolvemos NULL
		std::cout << "Error al realizar la multiplicación\n";
		return NULL;
	}
}

void clientMatrix::writeMatrix(matrix_t *m,std::string fileName){
	
	// mostrar la operación por pantalla
	std::cout << "Escribiendo en el archivo " << fileName << " la matriz:\n";
	printMatrix(m);

	// declaramos los buffers de entrada y salida
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
	
	// mostramos el resultado por pantalla
	if(ok){
		std::cout << "Matriz guarda correctamente\n";
	}
	else{
		std::cout << "Error al escribir la matriz en " << fileName << std::endl;
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
		// si el servidor valida la petición, desempaquetamos el resultado
		matrix_t* matrix=new matrix_t[1];
		matrix->rows = unpack<int>(buffIn);
		matrix->cols = unpack<int>(buffIn);
		matrix->data = new int[matrix->rows * matrix->cols];
		unpackv(buffIn, matrix->data, (matrix->rows * matrix->cols));
		
		// mostramos el resultado por pantalla
		std::cout << "La matriz identidad es:\n";
		printMatrix(matrix);

		return matrix;
	}
	else
	{
		// si el servidor dio errores, devolvemos NULL
		std::cout << "Error al crear la matriz identidad\n";
		return NULL;
	}
}

matrix_t *clientMatrix::createRandMatrix(int rows, int cols){
	
	std::cout << "Creando matriz aleatoria " << rows << "x" << cols << std::endl;
	
	// declaramos los buffers de entrada y salida
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
		// si el servidor valida la petición, desempaquetamos el resultado
		matrix_t* matrix = new matrix_t[1];
		matrix->rows = unpack<int>(buffIn);
		matrix->cols = unpack<int>(buffIn);
		matrix->data = new int[matrix->rows * matrix->cols];
		unpackv(buffIn, matrix->data, (matrix->rows * matrix->cols));

		// mostramos el resultado por pantalla
		std::cout << "La matriz aleatoria es:\n";
		printMatrix(matrix);

		return matrix;
	}
	else{
		// si el servidor dio errores, devolvemos NULL
		std::cout << "Error al crear la matriz aleatoria\n";
		return NULL;
	}
}

void printMatrix(matrix_t* m)
{	
	std::cout << std::endl;
	for(int i = 0; i < m->rows; i++)
	{	
		for(int j = 0; j < m->cols; j++)
		{
			std::cout << m->data[i*m->cols + j] << " ";
		}

		std::cout << std::endl;
	}
	std::cout << std::endl;
}
