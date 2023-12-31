#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "utils.h"
#include "operacionesMatrix.h"
#include "multmatrix.h"


void recibeOp(){
					
	std::vector<unsigned char> buffIn;
	std::vector<unsigned char> buffOut;
	buffIn.resize(0);
	buffOut.resize(0);
			
	rpcTipoOperacion op;
	auto clientId=getLastClientID();

	recvMSG(clientId, buffIn);
	op=unpack<rpcTipoOperacion>(buffIn);

	multMatrix* m = new multMatrix();
	
	switch(op){
		case constructorClientOp:
		{
			std::cout<<"invoco constructorOp\n";
			//Mensaje comprobacion
			pack<int>(buffOut,1);
		
		}break;
				
		case readOp:
		{
			std::string fileName;
			int datoLength=unpack<int>(buffIn);
			fileName.resize(datoLength);
			unpackv(buffIn, fileName.data(),datoLength + 1);
			
			matrix_t* res = new matrix_t[1];
			res = m->readMatrix(fileName.data());

			if(res != NULL){  //Si la matriz leida no da  error

				//Empaquetamos la matriz leida
				pack(buffOut, (int)1); //Se ha leido correctamente

				pack(buffOut,(int)res->rows);
				pack(buffOut,(int)res->cols);
				packv(buffOut,res->data,(res->rows * res->cols));	
			}else{
				std::cout << "No se ha podido leer la matriz\n";
				pack(buffOut, (int)0); //No se ha leido correctamente
			}
		}break;

		case multOp:
		{

			matrix_t* m1 = new matrix_t[1];	
			matrix_t* m2 = new matrix_t[1];			

			//Desempaquetamos la primera matriz	
			m1->rows = unpack<int>(buffIn);
			m1->cols = unpack<int>(buffIn);
			m1->data = new int[m1->rows * m1->cols];
			unpackv(buffIn, m1->data,(m1->rows * m1->cols));

			//Desempaquetamos la segunda matriz
			m2->rows = unpack<int>(buffIn);
			m2->cols = unpack<int>(buffIn);
			m2->data = new int[m2->rows * m2->cols];
			unpackv(buffIn, m2->data,(m2->rows * m2->cols));

			matrix_t* res = new matrix_t[1];
			res = m->multMatrices(m1,m2);

			if(res != NULL){   //Si la matriz resultante no da error

				//Empaquetamos respuesta
				pack(buffOut, (int)1); //Se ha realizado la multiplicación de forma correcta

				pack(buffOut, res->rows);
				pack(buffOut, res->cols);
				packv(buffOut, res->data, (res->rows * res->cols));
			}else{
				pack(buffOut, (int)0); //Error al multiplicar
			}

			
		}break;
				
		case writeOp:
		{
			matrix_t* m1 = new matrix_t[1];	

			m1->rows = unpack<int>(buffIn);
			m1->cols = unpack<int>(buffIn);
			m1->data = new int[m1->rows * m1->cols];
			
			std::string fileName;
			int datoLength=unpack<int>(buffIn);
			fileName.resize(datoLength);
			unpackv(buffIn, &fileName,datoLength);

			m->writeMatrix(m1,fileName.data());

			//Empaquetamos respuesta
			pack(buffOut, (int)1);

		}break;
			
		case identityOp:
		{
			matrix_t* m1 = new matrix_t[1];	

			m1->rows = unpack<int>(buffIn);
			m1->cols = unpack<int>(buffIn);
			m1->data = new int[m1->rows * m1->cols];

			m1 = m->createIdentity(m1->rows, m1->cols);

			//Empaquetamos respuesta
			pack(buffOut, (int)1);
			pack(buffOut, m1->rows);
			pack(buffOut, m1->cols);
			packv(buffOut, m1->data, (m1->rows * m1->cols));
				
		}break;
			
		case randOp:
		{					
			matrix_t* m1 = new matrix_t[1];	

			m1->rows = unpack<int>(buffIn);
			m1->cols = unpack<int>(buffIn);
			m1->data = new int[m1->rows * m1->cols];

			m1 = m->createRandMatrix(m1->rows, m1->cols);

			//Empaquetamos respuesta
			pack(buffOut, (int)1);
			pack(buffOut, m1->rows);
			pack(buffOut, m1->cols);
			packv(buffOut, m1->data, (m1->rows * m1->cols));
				
		}break;
			
		case destructorClientOp:
		{
				//Mensaje comprobante
				std::cout << "Conexión cerrada con el cliente\n";
				pack(buffOut, (int)1);	
				
		}break;
			
		default:
		{
					
			std::cout<<"ERROR:"<<__FILE__<<":"<<__LINE__<<" operación no definida\n";
			//return NO OK
			pack(buffOut,(int)0);

		}break;
	};

	//Enviamos respuesta
	sendMSG(clientId,buffOut);
}

int main(int argc, char** argv){

    std::cout<<">Servidor iniciado\n";
    int serverSocket=initServer(15000);

    while(1){
		//esperar conexión cliente
		while(!checkClient()){
			//trabajo útil
			usleep(1000);
		
		}
		//atender cliente
		recibeOp();
    }

    close(serverSocket);
	return 0;
}