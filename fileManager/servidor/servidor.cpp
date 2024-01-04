// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS  2023/2024

# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <thread>
# include "./utils.h"
# include "./conexion_cliente.h"
#include <fstream>

void atiendeCliente(int clientId, char *RUTA);	// función que atiende a un cliente
bool conectarBroker(char *IP_BROKER, int PUERTO_BROKER, char *IP, int PUERTO);	// función que conecta con el broker para registrarse como servidor

int main()
{
    // obtener la información del servidor del archivo data.txt
    std::ifstream dataFile("data.txt");
    
    // definir los valores por defecto
    int PUERTO = 15000;
    int PUERTO_BROKER = 15015;
    std::string IP_string = "127.0.0.1";
    std::string IP_BROKER_string = "127.0.0.1";
    std::string RUTA_string = "/home/antonio/Repos/remote-objects/filemanager/servidor/files";

    // leer los valores del archivo
    if(dataFile.peek() != std::ifstream::traits_type::eof() && dataFile.is_open())
    {
	dataFile >> IP_string;
	dataFile >> PUERTO;
	dataFile >> IP_BROKER_string;
	dataFile >> PUERTO_BROKER;
	dataFile >> RUTA_string;
    }
    
    // convertir las ip a char*
    char *IP = new char[IP_string.size()+1];
    char *IP_BROKER = new char[IP_BROKER_string.size()+1];
    char *RUTA = new char[RUTA_string.size()+1];
    std::copy(IP_string.begin(), IP_string.end(), IP);
    std::copy(IP_BROKER_string.begin(), IP_BROKER_string.end(), IP_BROKER);
    std::copy(RUTA_string.begin(), RUTA_string.end(), RUTA);

    dataFile.close();

    int serverSocket = initServer(PUERTO);

    std::cout << "Server iniciado en " << IP << ":" << PUERTO << "\n";
    std::cout << "Ruta de los archivos: " << RUTA << "\n";

    // conectar con el broker
    std::cout << "Conectando con el broker en " << IP_BROKER << ":" << PUERTO_BROKER << "\n";

    if(!conectarBroker(IP_BROKER, PUERTO_BROKER, IP, PUERTO))
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
		std::thread* th=new std::thread (atiendeCliente, clientId, RUTA);
    }

    close(serverSocket);

    return 0;
}

void atiendeCliente(int clientId, char *RUTA)
{
    // creamos un objeto de la clase ConexionCliente
    ConexionCliente c = ConexionCliente(clientId, RUTA);
 
    // mientras no cierre conexión
    do
    {
        // recibir paquete operación y atender operación
        c.recibeOp();
    }while(!c.connectionClosed());
}

bool conectarBroker(char *IP_BROKER, int PUERTO_BROKER, char *IP, int PUERTO)
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
