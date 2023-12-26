#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "./include/utils.h"
#include "./include/operaciones.h"
#include "./include/filemanager.h"

int main()
{
    int serverSocket = initServer(15000);

    std::cout<<"Server iniciado\n";
    std::cout<<"Esperando conexión...\n";

    FileManager *fileManager = new FileManager("/home/antonio/practica-SSDD/files");

    while(true)
    {
        //esperamos a que se conecte un cliente
        while(!checkClient())
        {
            usleep(1000);
        }

        //obtenemos el id del cliente
        auto clientId = getLastClientID();

		//recibir petición de operación
		std::vector<unsigned char> buffIn;
		std::vector<rpcInvocacion> rpcOps;
		
		std::vector<rpcResultado> buffOut;

		recvMSG(clientId,buffIn);

        //desempaqueta operación
        std::cout << "Cliente " << clientId << " - Desempaquetando operaciones\n";
		
		unsigned char numOps=unpack<unsigned char>(buffIn);
		rpcOps.resize(numOps);
		for(auto &op:rpcOps)
		{
			op=deserializaInvocacion(buffIn);
		}
		
		//invoco operación
        std::cout << "Cliente " << clientId << " - Invocando operaciones\n";

		for(auto &op: rpcOps)
		{
			switch(op.tipoOp)
			{
                case opListFiles:
                {

                    std::cout << "Cliente " << clientId <<" - Listando archivos\n";

                    rpcResultado resultado;
                    resultado.tipoOp=opListFiles;

                    // Rellenar el vector de archivos
                    resultado.listFiles.fileList = fileManager->listFiles();

                    buffOut.push_back(resultado);

                    fileManager->freeListedFiles(resultado.listFiles.fileList);
                }break;

                case opReadFile:
                {

                    std::cout << "Cliente " << clientId <<" - Leyendo archivo " << op.readFile.fileName << "\n";

                    rpcResultado resultado;
                    resultado.tipoOp=opReadFile;

                    fileManager->readFile(op.readFile.fileName, *resultado.readFile.data, *resultado.readFile.dataLength);

                    std::cout << "Cliente " << clientId <<" - Guardando en el buffer de salida\n";

                    buffOut.push_back(resultado);
                }break;

                case opWriteFile:
                {
                    rpcResultado resultado;
                    resultado.tipoOp=opWriteFile;
                    
                    fileManager->writeFile(op.writeFile.fileName, op.writeFile.data, op.writeFile.dataLength);

                    buffOut.push_back(resultado);
                }break;

				default:
					std::cout<<"Error:  Operación no definida\n";
				break;		
			}
		}
		
		//retorno resultado
		sendMSG(clientId,buffOut);

        std::cout<<"Cliente "<<clientId<<" desconectado\n";
		
		//cerrar cliente
		closeConnection(clientId);
    }

    close(serverSocket);

    return 0;
}
