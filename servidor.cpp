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

    FileManager fileManager = FileManager("/home/antonio/practica-SSDD/files");

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
		
		unsigned char numOps=unpack<unsigned char>(buffIn);
		rpcOps.resize(numOps);
		for(auto &op:rpcOps)
		{
			op=deserializaInvocacion(buffIn);
		}
		
		//invoco operación
		for(auto &op: rpcOps)
		{
			switch(op.tipoOp)
			{

                case opListFiles:
                {
                    rpcResultado resultado;
                    resultado.tipoOp=opListFiles;
                    
                    resultado.listFiles.fileList=fileManager.listFiles();

                    buffOut.push_back(resultado);
                }break;

                case opReadFile:
                {
                    rpcResultado resultado;
                    resultado.tipoOp=opReadFile;

                    fileManager.readFile(op.readFile.fileName, *resultado.readFile.data, *resultado.readFile.dataLength);
                    buffOut.push_back(resultado);
                }break;

                case opWriteFile:
                {
                    rpcResultado resultado;
                    resultado.tipoOp=opWriteFile;
                    
                    fileManager.writeFile(op.writeFile.fileName, op.writeFile.data, op.writeFile.dataLength);

                    buffOut.push_back(resultado);
                }break;

				default:
					std::cout<<"Error:  Operación no definida\n";
				break;		
			}
		}
		
		//retorno resultado
		sendMSG(clientId,buffOut);
		
		//cerrar cliente
		closeConnection(clientId);
    }

    close(serverSocket);

    return 0;
}
