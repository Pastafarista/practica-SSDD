#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "./include/utils.h"

int main()
{
    std::cout<<"Server iniciado\n";

    int serverSocket = initServer(15000);

    while(true)
    {
        //esperamos a que se conecte un cliente
        while(!checkClient())
        {
            usleep(1000);
        }

        //obtenemos el id del cliente
        auto clientId = getLastClientID();

        std::vector<unsigned char> buffIn;
        std::vector<unsigned char> buffOut;

        std::cout << "Cliente conectado: " << clientId << std::endl;

        //recibimos el mensaje del cliente
        recvMSG(clientId, buffIn);

        //mostramos el mensaje recibido
        std::cout << "Mensaje recibido: " << unpack<int>(buffIn) << std::endl;
    }

    close(serverSocket);

    return 0;
}
