// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS 2023/2024

# pragma once
# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <string>
# include "./utils.h"
# include "./operacionesMatrix.h"
# include "./multmatrix.h"
# define RUTA "/home/antonio/proyectos/PRACTICA-SSDD/multMatrix/server_files"

class ConexionCliente{
	private:
		int clientId = -1;
		bool terminarConexion = false;
		multMatrix* m = new multMatrix();

	public:

		ConexionCliente(int clientId)
		{
			if(clientId == -1)
				std::cout << "Error - ClientId no definido\n";
				
			this->clientId = clientId;
		};
		
		bool connectionClosed()
		{
			return this->terminarConexion;
		};

		~ConexionCliente()
		{
			delete m;
		};

		void recibeOp()
		{
			// recibimos la operación
			std::cout << "Cliente " << clientId << " - recibiendo operación\n";
			std::vector<unsigned char> buffIn;
			std::vector<unsigned char> buffOut;
			buffIn.resize(0);
			buffOut.resize(0);
			
			rpcTipoOperacion op;
			recvMSG(clientId, buffIn);
			op=unpack<rpcTipoOperacion>(buffIn);
			
			switch(op)
			{
				case constructorClientOp:
				{
					std::cout << "Cliente " << clientId << " - invocando constructor\n";
					pack<int>(buffOut, 1);
				}break;

				case readOp:
				{
					std::cout << "Cliente " << clientId << " - leyendo matriz\n";
			
					std::cout << "Desempaquetando tamaño\n";

					std::string fileName;
					int datoLength=unpack<int>(buffIn);

					std::cout << "El tamaño es " << datoLength << std::endl;

					std::cout << "Desempaquetando string\n";

					fileName.resize(datoLength);
					unpackv(buffIn, fileName.data(), datoLength + 1);
					
					std::cout << "Leyendo matriz\n";

					matrix_t* res = new matrix_t[1];
					res = m->readMatrix(fileName.data());

					if(res != NULL){  //Si la matriz leida no da  error

						//Empaquetamos la matriz leida
						pack(buffOut, (int)1); //Se ha leido correctamente

						pack(buffOut,(int)res->rows);
						pack(buffOut,(int)res->cols);
						packv(buffOut,res->data,(res->rows * res->cols));	
					}else{
						std::cout << "Cliente " << clientId << " - No se ha podido leer la matriz\n";
						pack(buffOut, (int)0); //No se ha leido correctamente
					}
				}break;

				case multOp:
				{
					std::cout << "Cliente " << clientId << " - multiplicando matrices\n";

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

					std::cout << "Cliente " << clientId << " - Guardando matrices\n";
					
					std::cout << "Creando matriz\n";

					matrix_t* m1 = new matrix_t[1];	
					
					std::cout << "Desempaquetando matriz\n";

					m1->rows = unpack<int>(buffIn);
					m1->cols = unpack<int>(buffIn);
					
					std::cout << "La matriz es " << m1->rows << "x" << m1->cols << std::endl;

					m1->data = new int[m1->rows * m1->cols];
					unpackv(buffIn, m1->data,(m1->rows * m1->cols));

					std::cout << "Desempaquetando tamaño\n";

					std::string fileName;
					int datoLength=unpack<int>(buffIn);

					std::cout << "Redimensionando string\n";

					fileName.resize(datoLength);

					std::cout << "Desempaquetando nombre\n";

					std::cout << "Tam: " << datoLength << std::endl;

					unpackv(buffIn, fileName.data(), datoLength);
					
					// guardamos la matriz en local
					m->writeMatrix(m1, fileName.data());

					// empaquetamos la respuesta
					pack(buffOut, (int)1);

				}break;
					
				case identityOp:
				{
					std::cout << "Cliente " << clientId << " - Guardando matriz identidad\n";

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
					std::cout << "Cliente " << clientId << " - Guardando matriz aleatoria\n";

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
						std::cout << "Cliente " << clientId << " - Conexión cerrada con el cliente\n";
						pack(buffOut, (int)1);	
						this->terminarConexion = true;	
				}break;
					
				default:
				{
							
					std::cout<<"ERROR:"<<__FILE__<<":"<<__LINE__<<" operación no definida\n";
					//return NO OK
					pack(buffOut,(int)0);

				}break;
			}

			//Enviamos respuesta
			std::cout << "Cliente " << clientId << " - enviando respuesta\n"; 
			sendMSG(clientId,buffOut);
		};
};


