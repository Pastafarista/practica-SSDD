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
	auto serverConnection=initClient("172.24.247.220", 15000);

    std::cout<<"Conexión exitosa\n";
	
	// crear invocación operacion
    rpcInvocacion op1, op2;
    op1.tipoOp=opListFiles;
    op1.listFiles.none=0;

    const char* fileName = "ejemplo.txt";

    op2.tipoOp=opReadFile;
    op2.readFile.fileName = (char *)fileName;
    	
	// enviar invocación
	std::vector<unsigned char> buffOut;
	std::vector<rpcResultado> buffIn;

    // empaquetar la operación
	pack<unsigned char>(buffOut, 1);
	serializaInvocacion(op2,buffOut);

    std::cout << "Enviando operación: leer archivo " <<  op2.readFile.fileName << "\n";

    // enviar la operación
	sendMSG(serverConnection.serverId,buffOut);

    std::cout << "Esperando respuesta del servidor...\n";

	// recibir resultados
	recvMSG(serverConnection.serverId,buffIn);

    std::cout << "Respuesta recibida, mostrando resultados:\n";

	// mostrar resultados
	{	for(auto &res:buffIn){
		
			switch(res.tipoOp)
			{

                case opListFiles:
                {
                    std::cout << "Listando archivos...\n";

                    std::vector<std::string*> *archivos;
                    archivos=res.listFiles.fileList;

                    std::cout << "Asignación correcta\n";

                    try{

                        // Ver si la lista de archivos está vacía

                        if(archivos->empty())
                        {
                            std::cout << "La lista de archivos está vacía\n";
                        }
                        else{
                            std::cout << "La lista de archivos no está vacía\n";
                        }

                        std::cout << "Tamaño de la lista: " << archivos->size() << std::endl;
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }

                    // Mostrar el contenido de la lista de archivos
                    for(unsigned int i=0;i<archivos->size();++i)
                    {
                        std::cout<<"Fichero: "<<archivos->at(i)->c_str()<<std::endl;
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
	}

	//cerrar conexión server
	closeConnection(serverConnection.serverId);
	return 0;
}
