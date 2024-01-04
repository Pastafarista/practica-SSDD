// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS  2023/2024

# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <string>
# include "utils.h"
# include "operacionesMatrix.h"
# include "multmatrix.h"
# include "conexion_cliente.h"
# include <fstream>

void atiendeCliente(int clientId); // función que atiende a un cliente
bool conectarBroker(char *IP, int PUERTO, char *IP_BROKER, int PUERTO_BROKER);           // función que conecta con el broker para registrarse como servidor

int main(){

    // obtener la información del servidor del archivo data.txt
    std::ifstream dataFile("data.txt");
    
    // definir los valores por defecto
    int PUERTO = 15000;
    int PUERTO_BROKER = 15015;
    std::string IP_string = "127.0.0.1";
    std::string IP_BROKER_string = "127.0.0.1";

    // leer los valores del archivo
    if(dataFile.peek() != std::ifstream::traits_type::eof() && dataFile.is_open())
    {
	dataFile >> IP_string;
	dataFile >> PUERTO;
	dataFile >> IP_BROKER_string;
	dataFile >> PUERTO_BROKER;
    }
    
    // convertir las ip a char*
    char *IP = new char[IP_string.size()+1];
    char *IP_BROKER = new char[IP_BROKER_string.size()+1];
    std::copy(IP_string.begin(), IP_string.end(), IP);
    std::copy(IP_BROKER_string.begin(), IP_BROKER_string.end(), IP_BROKER);

    dataFile.close();

    std::cout << "Servidor iniciado en el puerto " << PUERTO << std::endl;
    int serverSocket = initServer(PUERTO);
   		
    // conectar con el broker
    std::cout << "Conectando con el broker en " << IP_BROKER << ":" << PUERTO_BROKER << "\n";

    if(!conectarBroker(IP, PUERTO, IP_BROKER, PUERTO_BROKER))
    {
	return 0;
    }
    {
        return 0;
    }

    while(true){
		//esperar conexión cliente
		while(!checkClient()){
			//trabajo útil
			usleep(1000);
		
		}
		
        	// obtenemos el id del cliente
        	auto clientId = getLastClientID();
		
        	// creamos un hilo para atender al cliente
		std::thread* th=new std::thread (atiendeCliente,clientId);
    }

    close(serverSocket);
	return 0;
}

void atiendeCliente(int clientId)
{
	// creamos un objeto de la clase ConexionCliente
    ConexionCliente c = ConexionCliente(clientId);
 
    // mientras no cierre conexión
    do
    {
        // recibir paquete operación y atender operación
        c.recibeOp();
    }while(!c.connectionClosed());
}

bool conectarBroker(char *IP, int PUERTO, char *IP_BROKER, int PUERTO_BROKER)
{
    // conectar con el broker
    auto serverConnection = initClient(IP_BROKER, PUERTO_BROKER);

    // comprobar si se ha podido conectar
    if(serverConnection.socket == -1)
    {
        std::cout << "Error al conectar con el broker\n";
        return false;
    }

    // enviar petición de operación
    std::vector<unsigned char> buffOut;

    // empaquetar operación
    pack(buffOut, opConnectServer); // operación de conexión
    
    // empaquetar ip
    std::string ip = IP;
    int dataLength = ip.size()+1;

    pack(buffOut, dataLength);
    packv(buffOut, ip.data(), dataLength);

    // empaquetar puerto
    pack(buffOut, PUERTO);

    // empaquetar tipo de servidor
    pack(buffOut, tipoMultmatrix);

    // enviar operación
    sendMSG(serverConnection.serverId, buffOut);

    // recibir respuesta
    std::vector<unsigned char> buffIn; 

    recvMSG(serverConnection.serverId, buffIn);

    int ok = unpack<int>(buffIn);

    if(ok)
    {
        std::cout << "Conectado correctamente con el broker\n";
        return true;
    }
    else
    {
        std::cout << "Error al conectar con el broker\n";
        return false;
    }
}
