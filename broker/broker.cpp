// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS  2023/2024

# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include "./utils.h"
# include "./operaciones.h"
# define PUERTO 15015

class fichaServidor
{
    public:
        std::string ip;
        int puerto;
        tipoServidor tipo;

        void print()
        {
            std::cout << "Ip: " << ip << "\n";
            std::cout << "Puerto: " << puerto << "\n";
            
            if(tipo == tipoFilemanager)
                std::cout << "Tipo: filemanager\n";
            else if(tipo = tipoMultmatrix)
                std::cout << "Tipo: multmatrix\n";
            else
                std::cout << "Tipo: desconocido\n";
        }
};

int main()
{
    // iniciamos el servidor
    int serverSocket = initServer(PUERTO);

    std::cout << "Broker iniciado en el puerto " << PUERTO << "\n";
    std::cout << "Esperando conexión...\n";

    std::map<int, fichaServidor> servidores;

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
        std::cout << "Cliente " << clientId << " - (?) recibiendo operación\n";

        std::vector<unsigned char> buffIn;
        std::vector<unsigned char> buffOut;
        opBroker op;
        recvMSG(clientId, buffIn);

        // desempaquetamos la operación
        op=unpack<opBroker>(buffIn);

        // procesamos la operación
        switch(op)
        {
            case opConnectServer:
            {
                std::cout << "Cliente " << clientId << " (servidor de objetos) - Conectando\n";

                // desempaquetamos la ip                
                std::string ip;
                int ipLength = unpack<int>(buffIn);
                ip.resize(ipLength);
                unpackv(buffIn, ip.data(), ipLength);

                // desempaquetamos el puerto
                int puerto = unpack<int>(buffIn);

                // desempaquetamos el tipo de servidor
                tipoServidor tipo = unpack<tipoServidor>(buffIn);

                // guardamos los datos del servidor de objetos
                fichaServidor fc;
                fc.ip = ip;
                fc.puerto = puerto;
                fc.tipo = tipo;

                servidores[clientId] = fc;

                // mostrar datos del cliente
                std::cout << "Cliente " << clientId << " (servidor de objetos) - datos:\n";
                fc.print();

                // enviar respuesta
                pack(buffOut,(int)1);
            }
            break;

            case opConnectClient:
            {
                std::cout << "Cliente " << clientId << " (cliente) - Conectando\n";

                if(servidores.size() == 0)
                {
                    std::cout << "Cliente " << clientId << " (cliente) - No hay servidores de objetos conectados\n";
                    pack(buffOut,(int)0);
                    break;
                }
                else
                {
                    // desempaquetemaos el tipo de cliente
                    tipoServidor tipo = unpack<tipoServidor>(buffIn);
			
		    // mostrar por pantalla los datos del cliente
		    std::cout << "Cliente " << clientId << " (cliente) - datos:\n";
		    if(tipo == tipoFilemanager)
		    {
			std::cout << "Tipo: filemanager\n";
		    }
		    else if(tipo == tipoMultmatrix)
		    {
			std::cout << "Tipo: multmatrix\n";
		    }
		    else{
			std::cout << "Tipo: desconocido\n";
		    }

                    // buscar un servidor de objetos del tipo solicitado
                    int serverId = -1;
                    for(auto it = servidores.begin(); it != servidores.end(); it++)
                    {
                        if(it->second.tipo == tipo)
                        {
                            serverId = it->first;
                            break;
                        }
                    }

                    if(serverId == -1)
                    {
                        std::cout << "Cliente " << clientId << " (cliente) - No hay servidores de objetos del tipo solicitado\n";
                        pack(buffOut,(int)0);
                        break;
                    }

                    // Enviar respuesta
                    std::cout << "Cliente " << clientId << " (cliente) - Conectado con el servidor de objetos " << servidores[serverId].ip << ":" << servidores[serverId].puerto << "\n";

                    // empaquetar respuesta
                    pack(buffOut,(int)1);

                    // empaquetar ip
                    std::string ip = servidores[serverId].ip;
                    int ipLength = ip.size()+1;
                    pack(buffOut, ipLength);
                    packv(buffOut, ip.data(), ipLength);

                    // empaquetar puerto
                    pack(buffOut, servidores[serverId].puerto);
                }
            }  
	    break;

            default:
            {
                std::cout << "Cliente " << clientId << " - (?) Operación desconocida\n";

                // enviar respuesta
                pack(buffOut,(int)0);
            }
            break;
        }

        //enviamos la respuesta
        std::cout << "Cliente " << clientId << " - (?) Enviando respuesta\n";
        sendMSG(clientId, buffOut);
    }

    return 0;
}
