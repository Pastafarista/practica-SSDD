#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "./include/utils.h"
#include "./include/operaciones.h"
#include "./include/filemanager.h"

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
		

        // recibimos la operación
        std::cout << "Cliente " << clientId << " - recibiendo operación\n";

		std::vector<unsigned char> buffIn;
		std::vector<unsigned char> buffOut;
		tipoOperacion op;
		recvMSG(clientId, buffIn);
		op=unpack<tipoOperacion>(buffIn);

        // procesamos la operación
        switch(op)
        {
            case opListFiles:
            {
                std::cout << "Cliente " << clientId << " - Listando ficheros\n";

                // leer lista de ficheros
                std::vector<std::string*>* fileList = fileManager->listFiles();

                // empaquetar respuesta
                pack(buffOut,(int)1);
                pack(buffOut,(int)fileList->size());
                for(auto fileName : *fileList)
                {
                    pack(buffOut,(int)(fileName->length()+1));
                    packv(buffOut,fileName->data(),(int)(fileName->length()+1));
                }

                // liberar memoria
                fileManager->freeListedFiles(fileList);
            }
            break;

            case opReadFile:
            {
                std::cout << "Cliente " << clientId << " - Leyendo fichero\n";

                // recibir nombre del fichero
                std::string fileName;
                int fileNameLength = unpack<int>(buffIn);
                fileName.resize(fileNameLength);
                unpackv(buffIn,fileName.data(),fileNameLength);

                // comprobar si existe el fichero
                if(!fileManager->fileExists((char*)fileName.data()))
                {
                    std::cout << "Cliente " << clientId << " - El fichero no existe\n";
                    pack(buffOut,(int)0);
                    break;
                }

                // leer fichero
                std::cout << "Cliente " << clientId << " - Leyendo fichero " << fileName << "\n";
                char* data;
                unsigned long int dataLength;
                fileManager->readFile((char*)fileName.data(),data,dataLength);

                // empaquetar respuesta
                pack(buffOut,(int)1);
                pack(buffOut,(int)dataLength);
                packv(buffOut,data,dataLength);

                // liberar memoria
                delete[] data;
            }
            break;

            case opWriteFile:
            {
                std::cout << "Cliente " << clientId << " - Escribiendo fichero\n";
            }
            break;

            default:
            {
                    std::cout<<"ERROR:" << __FILE__ << ":" << __LINE__ << " operación no definida\n";

					// enviar respuesta de error
					pack(buffOut,(int)0);
            }
            break;
        }

        //enviamos la respuesta
        std::cout << "Cliente " << clientId << " - Enviando respuesta\n";
        sendMSG(clientId,buffOut);


        std::cout << "Cliente " << clientId << " desconectado\n";
		
		//cerrar cliente
		closeConnection(clientId);
    }

    close(serverSocket);

    return 0;
}
