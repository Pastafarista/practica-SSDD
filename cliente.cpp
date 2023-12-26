# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include "./include/utils.h"
# include "./include/operaciones.h"

void listFiles(unsigned int serverId)
{
    std::cout<<"Listando ficheros...\n";
    //enviar petición de operación
    std::vector<unsigned char> buffOut;
    pack<tipoOperacion>(buffOut, opListFiles);

    //enviar operación
    sendMSG(serverId, buffOut);

    //recibir respuesta
    std::vector<unsigned char> buffIn;
    recvMSG(serverId, buffIn);

    int ok = unpack<int>(buffIn);

    if(ok)
    {
        std::cout<<"Respuesta correcta\n";
    }
    else
    {
        std::cout<<"Respuesta incorrecta\n";
    }

    //desempaquetar respuesta
    /*int numFiles = unpack<int>(buffIn);
    std::cout<<"Hay "<<numFiles<<" ficheros\n";
    for(int i=0; i<numFiles; i++)
    {
        std::vector<unsigned char> buffIn;
        recvMSG(serverId, buffIn);
        std::string fileName = unpack<std::string>(buffIn);
        std::cout<<fileName<<"\n";
    }*/
}

int main(int argc, char const *argv[])
{

    std::cout<<"Cliente iniciado\n";

    std::cout << "Conectando con el servidor...\n";

    //iniciar conexión server
	auto serverConnection=initClient("172.24.247.220", 15000);

    std::cout<<"Conexión exitosa\n";

    //lógica
    listFiles(serverConnection.serverId);

	//cerrar conexión server
	closeConnection(serverConnection.serverId);
	return 0;
}
