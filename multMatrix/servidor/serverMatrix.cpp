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

// constantes
# define PUERTO 15030
# define PUERTO_BROKER 15015
const char *IP = "127.0.0.1";
const char *IP_BROKER = "127.0.0.1";

void atiendeCliente(int clientId); // función que atiende a un cliente
bool conectarBroker();           // función que conecta con el broker para registrarse como servidor

int main(int argc, char** argv){
	
    std::cout << "Servidor iniciado en el puerto " << PUERTO << std::endl;
    int serverSocket = initServer(PUERTO);
		
    // conectar con el broker
    std::cout << "Conectando con el broker en " << IP_BROKER << ":" << PUERTO_BROKER << "\n";

    if(!conectarBroker())
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

bool conectarBroker()
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
