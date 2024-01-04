// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS  2023/2024

# include <stdio.h>
# include <stdlib.h>
# include <sstream>
# include <iostream>
# include <fstream>
# include "./utils.h"
# include "./operaciones.h"
# include "./filemanager.h"

void endConnection(unsigned int serverId);  // función que cierra la conexión con el servidor
void listFiles(unsigned int serverId);    // función que lista los ficheros del servidor
void readFile(unsigned int serverId, std::string fileName); // función que lee un fichero del servidor
void writeFile(unsigned int serverId, std::string fileName, std::string data);  // función que escribe un fichero en el servidor
void uploadFile(unsigned int serverId, std::string fileName, char *RUTA);   // función que sube un fichero al servidor
void downloadFile(unsigned int serverId, std::string fileName, char *RUTA); // función que descarga un fichero del servidor
void process(std::string line, unsigned int serverId, char *RUTA);  // función que procesa un comando introducido por el usuario
bool conectarBroker(char *IP_BROKER, int PUERTO_BROKER, char* &IP, int &PUERTO);       // función que conecta con el broker para obtener los datos del servidor (ip y puerto)

int main(int argc, char const *argv[])
{
    // obtener la información del servidor del archivo data.txt
    std::ifstream dataFile("data.txt");
    
    // definir los valores por defecto
    int PUERTO_BROKER = 15015;
    std::string IP_BROKER_string = "127.0.0.1";
    std::string RUTA_string = "/home/antonio/Repos/remote-objects/fileManager/cliente/files";

    // leer los valores del archivo
    if(dataFile.peek() != std::ifstream::traits_type::eof() && dataFile.is_open())
    {
	dataFile >> IP_BROKER_string;
	dataFile >> PUERTO_BROKER;
	dataFile >> RUTA_string;
    }
    
    // convertir las ip a char*
    char *IP_BROKER = new char[IP_BROKER_string.size()+1];
    std::copy(IP_BROKER_string.begin(), IP_BROKER_string.end(), IP_BROKER);
    
    char *RUTA = new char[RUTA_string.size()+1];
    std::copy(RUTA_string.begin(), RUTA_string.end(), RUTA);

    dataFile.close();

    std::cout << "Cliente iniciado\n";
    std::cout << "Ruta de los ficheros: " << RUTA << "\n";
    std::cout << "Conectando con el broker en " << IP_BROKER << ":" << PUERTO_BROKER << "...\n";

    char *IP;
    int PUERTO;

    // conectar con el broker
    if(!conectarBroker(IP_BROKER, PUERTO_BROKER, IP, PUERTO))
    {
        return 0;
    }

    std::cout << "Conectando con el servidor " << IP << ":" << PUERTO << "...\n";

    // iniciar conexión server
	auto serverConnection = initClient(IP, PUERTO);

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
        if (!line.empty()) { process(line, serverConnection.serverId, RUTA); }
    }

	//cerrar conexión server
	closeConnection(serverConnection.serverId);
	return 0;
}


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

void writeFile(unsigned int serverId, std::string fileName, std::string data)
{
    std::cout<<"Escribiendo fichero...\n";

    std::vector<unsigned char> buffOut;

    // empaquetar operación
    pack<tipoOperacion>(buffOut, opWriteFile); // tipo de operación
    pack(buffOut,(int)(fileName.length()+1)); // tamaño del nombre del fichero
    packv(buffOut,fileName.data(),(int)(fileName.length()+1)); // nombre del fichero
    pack(buffOut,(int)(data.length()+1)); // tamaño de los datos
    packv(buffOut,data.data(),(int)(data.length()+1)); // datos

    //enviar operación
    sendMSG(serverId, buffOut);

    // recibir respuesta
    std::vector<unsigned char> buffIn;
    recvMSG(serverId, buffIn);

    // desempaquetar respuesta
    int ok = unpack<int>(buffIn);

    if(ok)
    {
        std::cout << "Archivo " << fileName << " escrito correctamente\n";
    }
    else
    {
        std::cout << "Error al escribir el archivo " << fileName << "\n";
    }
}

void uploadFile(unsigned int serverId, std::string fileName, char *RUTA)
{
    std::cout<<"Subiendo fichero " << fileName  << "...\n";

    FileManager* fileManager = new FileManager(RUTA);

    // comprobar si existe el fichero

    if(!fileManager->fileExists((char*)fileName.data()))
    {
        std::cout << "El fichero " << RUTA << "/" << fileName << " no existe\n";
        return;
    }

    // leer fichero
    char* data;
    unsigned long int dataLength;
    fileManager->readFile((char*)fileName.data(),data,dataLength);

    // escribir fichero
    writeFile(serverId, fileName, data);
}

void downloadFile(unsigned int serverId, std::string fileName, char *RUTA)
{
    std::cout<<"Descargando fichero " << fileName  << "...\n";

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
        
        FileManager fileManager(RUTA);

        // escribir fichero
        fileManager.writeFile((char*)fileName.data(), data, dataLength);

        std::cout << "El fichero" << fileName << " ha sido descargado correctamente en " << RUTA << "/" << fileName << "\n";

        delete[] data;
    }
    else
    {
        std::cout << "Error al descargar el archivo " << fileName << "\n";
    }
}

void process(std::string line, unsigned int serverId, char *RUTA)
{
    // separar comando
    std::string tmp; 
    std::stringstream ss(line);
    std::vector<std::string> words;

    while(getline(ss, tmp, ' ')){
        words.push_back(tmp);
    }

    std::string command = words[0];

    if (command == "ls")
    {
        listFiles(serverId);
    }
    else if(command == "cat")
    {
        // comprobar si se ha introducido un nombre de fichero
        if(words.size() != 2)
        {
            std::cout << "Uso: cat <nombre_fichero>\n";
            return;
        }
        else{
            readFile(serverId, words[1]);
        }
    }
    else if(command == "write")
    {
        // comprobar si se ha introducido un nombre de fichero
        if(words.size() < 3)
        {
            std::cout << "Uso: write <nombre_fichero> <data>\n";
            return;
        }
        else{            
            
            // juntar todos los argumentos siguientes en un string
            std::string data = "";

            for(int i = 2; i < words.size(); i++)
            {
                data += words[i];
                if(i != words.size()-1)
                {
                    data += " ";
                }
            }

            // escribir fichero
            writeFile(serverId, words[1], data);
        }
    }
    else if(command == "exit")
    {
        std::cout << "Cerrando cliente...\n";
        endConnection(serverId);
        exit(0);
    }
    else if(command == "clear")
    {
        system("clear");
    }
    else if(command == "upload")
    {
        // comprobar si se ha introducido un nombre de fichero
        if(words.size() != 2)
        {
            std::cout << "Uso: upload <nombre_fichero>\n";
            return;
        }
        else{
            uploadFile(serverId, words[1], RUTA);
        }
    }
    else if(command == "download")
    {
        // comprobar si se ha introducido un nombre de fichero
        if(words.size() != 2)
        {
            std::cout << "Uso: download <nombre_fichero>\n";
            return;
        }
        else{
            downloadFile(serverId, words[1], RUTA); 
        }
    }
    else if(command == "help")
    {
        std::cout << "Comandos disponibles:\n";
        std::cout << "ls: Listar ficheros del servidor\n";
        std::cout << "cat: Leer fichero del servidor\n";
        std::cout << "write: Escribir fichero en el servidor\n";
        std::cout << "upload: Subir fichero al servidor\n";
        std::cout << "download: Descargar fichero del servidor\n";
        std::cout << "clear: Limpiar la pantalla\n";
        std::cout << "exit: Salir del programa\n";
    }
    else
    {
        std::cout << "Comando no reconocido. Escriba 'help' para ver los comandos disponibles\n";
    }
}

bool conectarBroker(char *IP_BROKER, int PUERTO_BROKER, char* &IP, int &PUERTO){
    auto serverConnection = initClient(IP_BROKER, PUERTO_BROKER);

    // comprobar si se ha podido conectar
    if(serverConnection.socket == -1)
    {
        std::cout << "Error al conectar con el broker (servidor del broker no responde)\n";
        return false;
    }

    // enviar petición de operación
    std::vector<unsigned char> buffOut;

    // empaquetar operación
    pack(buffOut, opConnectClient); // operación de conexión

    // empaquetar tipo de objetos
    pack(buffOut, tipoFilemanager);

    // enviar operación
    sendMSG(serverConnection.serverId, buffOut);

    // recibir respuesta
    std::vector<unsigned char> buffIn;
    recvMSG(serverConnection.serverId, buffIn);

    // desempaquetar respuesta
    int ok = unpack<int>(buffIn);

    if(ok)
    {
        std::cout << "Conexión con el broker establecida\n";

        // desempaquetar ip
        std::string ip;
        int ipLength = unpack<int>(buffIn);
        ip.resize(ipLength);
        unpackv(buffIn, (char *)ip.data(), ipLength);
        
        // desempaquetar puerto
        int puerto = unpack<int>(buffIn);

        // guardar datos del servidor de objetos
        IP = new char[ipLength];
        strcpy(IP, ip.data());
        PUERTO = puerto;

        return true;
    }
    else
    {
        std::cout << "Error al conectar con el broker (no hay servidores de objetos)\n";
        return false;
    }
}
