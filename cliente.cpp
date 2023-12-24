# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include "./include/utils.h"
# include "./include/operaciones.h"

int main(int argc, char const *argv[])
{

    std::cout<<"Cliente iniciado\n";

    std::cout << "Conectando con el servidor...\n";

    //iniciar conexión server
	auto serverConnection=initClient("172.24.247.220",15000);

    std::cout<<"Conexión exitosa\n";
	
	// crear invocación operacion
    rpcInvocacion op1;
    op1.tipoOp=opListFiles;
    op1.listFiles.none=0;
	
	// enviar invocación
	std::vector<unsigned char> buffOut;
	std::vector<rpcResultado> buffIn;

    // empaquetar la operación
	pack<unsigned char>(buffOut,1);
	serializaInvocacion(op1,buffOut);

    std::cout << "Enviando operación\n";

    // enviar la operación
	sendMSG(serverConnection.serverId,buffOut);

	// recibir resultados
	recvMSG(serverConnection.serverId,buffIn);

	// mostrar resultados
	{	for(auto &res:buffIn){
		
			switch(res.tipoOp)
			{

                case opListFiles:
                {

                    // Mostrar el contenido de la lista de archivos

                    std::cout<<"Los archivos son: \n";

                    std::vector<std::string*> archivos;

                    archivos=*res.listFiles.fileList;

                    for (auto &archivo:archivos)
                    {
                        std::cout<<archivo<<"\n";
                    }

                }break;

                case opReadFile:
                {
                    std::cout<<"Contenido del arhivo: \n";
                    
                    for(int i=0; i<*res.readFile.dataLength; i++)
                    {
                        std::cout << res.readFile.data[i];
                    }

                    std::cout<<"\n";

                }

                case opWriteFile:
                {
                    if (res.writeFile.res == 0)
                    {
                        std::cout<<"El archivo se ha escrito correctamente\n";
                    }
                    else
                    {
                        std::cout<<"El archivo no se ha escrito correctamente\n";
                    }
                }break;
			
				default:
					std::cout<<"Error:  Operación no definida\n";
				break;		
			}
		}
	}/*else{
		std::cout<<"ERROR: operaciones no válidas\n";
	}*/
	//cerrar conexión server
	closeConnection(serverConnection.serverId);
	return 0;
}
