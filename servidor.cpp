#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "./utils.h"

int main(int argc, char const *argv[])
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
    }

    close(serverSocket);

    return 0;
}
