# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include "./include/utils.h"
# include "./include/operaciones.h"

int main(int argc, char const *argv[])
{
    //iniciar conexión server
	auto serverConnection=initClient("172.31.58.139",15000);

    std::cout<<"Cliente iniciado\n";
	
	// crear invocación operacion
    rpcInvocacion op1;
    op1.tipoOp=listFiles;
    op1.listFiles.none=0;
	
	// enviar invocación
	std::vector<unsigned char> buffOut;
	std::vector<rpcResultado> buffIn;

    // empaquetar la operación
	pack<unsigned char>(buffOut,1);
	serializaInvocacion(op1,buffOut);

    // enviar la operación
	sendMSG(serverConnection.serverId,buffOut);

	// recibir resultados
	recvMSG(serverConnection.serverId,buffIn);

	// mostrar resultados
	{	for(auto &res:buffIn){
		
			switch(res.tipoOp)
			{

                case listFiles:
                {
                    std::cout<<"Los archivos son: \n";

                    std::vector<std::string*> archivos;

                    archivos=*res.listFiles.fileList;

                    for (auto &archivo:archivos)
                    {
                        std::cout<<*archivo<<"\n";
                    }
                    
                }break;

                case readFile:
                {

                    std::cout<<"Contenido del arhivo: \n";
                    
                    for(auto &file:res.readFile.data)
                    {
                        std::cout<<file;
                    }

                }

                case writeFile:
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
