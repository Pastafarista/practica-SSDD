# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include "./include/utils.h"

int main(int argc, char const *argv[])
{
    
    std::cout<<"Cliente iniciado\n";

    auto serverConnection=initClient("127.0.0.1", 15000);

    /* Enviar mensaje al servidor */

    // Creamos un buffer de salida
    std::vector<unsigned char> buffOut;

    // Empaquetamos el mensaje

    int msg = 33;

	pack<int>(buffOut, msg);
	
    // Enviamos el mensaje
	sendMSG(serverConnection.serverId, buffOut);

    return 0;
}
