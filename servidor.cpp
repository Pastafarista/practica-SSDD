// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS  2023/2024

# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <thread>
# include "./include/utils.h"
# include "./include/conexion_cliente.h"
# define IP "172.24.247.220"
# define PUERTO 15000
# define IP_BROKER "172.24.247.220"
# define PUERTO_BROKER 15015

void atiendeCliente(int clientId); // función que atiende a un cliente
bool conectarBroker();           // función que conecta con el broker para registrarse como servidor

int main()
{
    int serverSocket = initServer(PUERTO);

    std::cout << "Server iniciado en " << IP << ":" << PUERTO << "\n";

    // conectar con el broker
    std::cout << "Conectando con el broker en " << IP_BROKER << ":" << PUERTO_BROKER << "\n";

    if(!conectarBroker())
    {
        return 0;
    }

    std::cout << "Esperando conexión...\n";

    while(true)
    {
        //esperamos a que se conecte un cliente
        while(!checkClient())
        {
            usleep(1000);
        }

        // obtenemos el id del cliente
        auto clientId = getLastClientID();
		
        // creamos un hilo para atender al cliente
		std::thread* th=new std::thread (atiendeCliente,clientId);
    }

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
    pack(buffOut, tipoFilemanager);

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