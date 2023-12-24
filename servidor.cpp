#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "./include/utils.h"
#include "./include/operaciones.h"
#include "./fileManager/filemanager.h"

int main()
{
    std::cout<<"Server iniciado\n";

    int serverSocket = initServer(15000);

    FileManager fileManager("./files");

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

                case listFiles:
                {
                    rpcResultado resultado;
                    resultado.tipoOp=listFiles;
                    
                    resultado.listFiles.fileList=fileManager.listFiles();

                    buffOut.push_back(resultado);
                }break;

                case readFile:
                {
                    rpcResultado resultado;
                    resultado.tipoOp=readFile;

                    fileManager.readFile(op.readFile.fileName, resultado.readFile.data, resultado.readFile.dataLength);
                    buffOut.push_back(resultado);
                }break;

                case writeFile:
                {
                    rpcResultado resultado;
                    resultado.tipoOp=writeFile;
                    
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
