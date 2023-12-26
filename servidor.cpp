#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include "./include/utils.h"
#include "./include/conexion_cliente.h"

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

int main()
{
    int serverSocket = initServer(15000);

    std::cout << "Server iniciado\n";
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
