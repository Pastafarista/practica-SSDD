# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <string>
# include "./include/utils.h"
# include "./include/operaciones.h"

void endConnection(unsigned int serverId){
    std::cout << "Cerrando conexión...\n";

    // enviar petición de operación
    std::vector<unsigned char> buffOut;
    pack<tipoOperacion>(buffOut, opEndConnection);

    // enviar operación
    sendMSG(serverId, buffOut);

    // recibir respuesta
    std::vector<unsigned char> buffIn;
    recvMSG(serverId, buffIn);

    int ok = unpack<int>(buffIn);

    if(ok)
    {
        std::cout << "Conexión cerrada\n";
    }
    else
    {
        std::cout << "Error al cerrar la conexión\n";
    }
}

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
        int numFiles = unpack<int>(buffIn);
        std::cout << "El servidor contiene " << numFiles << " ficheros:\n";
        for(int i=0;i<numFiles;i++)
        {
            int fileNameLength = unpack<int>(buffIn);
            char* fileName = new char[fileNameLength];
            unpackv(buffIn,fileName,fileNameLength);
            std::cout << fileName << "\n";
            delete[] fileName;
        }
    }
    else
    {
        std::cout << "Error al leer el listado de archivos\n";
    }
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
        int dataLength = unpack<int>(buffIn);
        char* data = new char[dataLength];
        unpackv(buffIn,data,dataLength);
        std::cout << "El fichero contiene:\n" << data << "\n";
        delete[] data;
    }
    else
    {
        std::cout << "Error al leer el archivo " << fileName << "\n";
    }
}

void process(std::string line, unsigned int serverId)
{

    // separar comando y argumentos
    std::string command = line.substr(0, line.find(" "));
    std::string args = line.substr(line.find(" ")+1);

    if (command == "ls")
    {
        listFiles(serverId);
    }
    else if(command == "cat")
    {
        // comprobar si se ha introducido un nombre de fichero
        if(args.empty() || line.find(" ") == std::string::npos)
        {
            std::cout << "Uso: cat <nombre_fichero>\n";
            return;
        }
        else{
            readFile(serverId, args);
        }
    }
    else if(command == "exit")
    {
        std::cout << "Cerrando cliente...\n";
        endConnection(serverId);
        exit(0);
    }
    else if(command == "help")
    {
        std::cout << "Comandos disponibles:\n";
        std::cout << "ls: Listar ficheros del servidor\n";
        std::cout << "cat: Leer fichero del servidor\n";
        std::cout << "exit: Salir del programa\n";
    }
    else
    {
        std::cout << "Comando no reconocido. Escriba 'help' para ver los comandos disponibles\n";
    }
}

int main(int argc, char const *argv[])
{

    std::cout << "Cliente iniciado\n";

    std::cout << "Conectando con el servidor...\n";

    // iniciar conexión server
	auto serverConnection = initClient("172.24.247.220", 15000);

    // comprobar si se ha podido conectar
    if(serverConnection.socket == -1)
    {
        std::cout << "Error al conectar con el servidor\n";
        return 0;
    }

    std::cout << "Conexión exitosa\n";

    //lógica
    bool exit = false;
    char* comando;

    for (std::string line; std::cout << "APP > " && std::getline(std::cin, line); )
    {
        if (!line.empty()) { process(line, serverConnection.serverId); }
    }

	//cerrar conexión server
	closeConnection(serverConnection.serverId);
	return 0;
}
