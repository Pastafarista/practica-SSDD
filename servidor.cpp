#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "./include/utils.h"
#include "./include/conexion_cliente.h"

int main()
{
    int serverSocket = initServer(15000);

    std::cout << "Server iniciado\n";
    std::cout << "Esperando conexión...\n";

    FileManager *fileManager = new FileManager("/home/antonio/practica-SSDD/files");

    while(true)
    {
        //esperamos a que se conecte un cliente
        while(!checkClient())
        {
            usleep(1000);
        }

        // obtenemos el id del cliente
        auto clientId = getLastClientID();
		
        // creamos un objeto de la clase ConexionCliente
        ConexionCliente c = ConexionCliente(clientId);
 
        // mientras no cierre conexión
        do
        {
            // recibir paquete operación y atender operación
            c.recibeOp();
        }while(!c.connectionClosed());
    }

    return 0;
}
