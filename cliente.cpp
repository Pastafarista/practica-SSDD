# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include "./include/utils.h"
# include "./include/operaciones.h"

void listFiles(unsigned int serverId)
{
    std::cout<<"Listando ficheros...\n";

    // enviar petición de operación
    std::vector<unsigned char> buffOut;
    pack<tipoOperacion>(buffOut, opListFiles);

    // enviar operación
    sendMSG(serverId, buffOut);

    // recibir respuesta
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

void readFile(unsigned int serverId, std::string fileName)
{
    std::cout<<"Leyendo fichero...\n";

    std::vector<unsigned char> buffOut;

    // empaquetar operación
    pack<tipoOperacion>(buffOut, opReadFile); // tipo de operación
    pack(buffOut,(int)(fileName.length()+1)); // tamaño del nombre del fichero
	packv(buffOut,fileName.data(),(int)(fileName.length()+1)); // nombre del fichero

    //enviar operación
    sendMSG(serverId, buffOut);

    // recibir respuesta
    std::vector<unsigned char> buffIn;
    recvMSG(serverId, buffIn);

    // desempaquetar respuesta

    int ok = unpack<int>(buffIn);

    if(ok)
    {
        std::cout << "Respuesta correcta\n";
        int dataLength = unpack<int>(buffIn);
        char* data = new char[dataLength];
        unpackv(buffIn,data,dataLength);
        std::cout << "El fichero contiene:\n"<<data<<"\n";
        delete[] data;
    }
    else
    {
        std::cout<<"Respuesta incorrecta\n";
    }
}

int main(int argc, char const *argv[])
{

    std::cout << "Cliente iniciado\n";

    std::cout << "Conectando con el servidor...\n";

    //iniciar conexión server
	auto serverConnection=initClient("172.24.247.220", 15000);

    std::cout<<"Conexión exitosa\n";

    //lógica
    readFile(serverConnection.serverId, "ejemplo.txt");

	//cerrar conexión server
	closeConnection(serverConnection.serverId);
	return 0;
}
