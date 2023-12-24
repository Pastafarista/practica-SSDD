#pragma once

typedef enum{
	opListFiles=1,
	opReadFile=2,
	opWriteFile=3
}rpcTipoOperacion;

typedef struct __attribute__ ((packed)){
	rpcTipoOperacion tipoOp;
	union{

		struct{
			char *none;
		}listFiles;

		struct
		{
			char *fileName;
		}readFile;

		struct{
			char *fileName;
			char *data;
			unsigned long int dataLength;
		}writeFile;
	};
}rpcInvocacion;


typedef struct __attribute__ ((packed)){
	rpcTipoOperacion tipoOp;
	union{

		struct{
			std::vector<std::string*>* fileList;
		}listFiles;

		struct{
			char **data;
			unsigned long int *dataLength;
		}readFile;

		struct{
			int res;
		}writeFile;
	};		
}rpcResultado;


template <typename T>


void pack(std::vector<unsigned char> &packet, T data)
{
	unsigned char* ptr=(unsigned char*)&data;
	int numBytes=sizeof(T);
	int size=packet.size();
	packet.resize(size+numBytes);
	std::copy(ptr,ptr+numBytes,packet.begin()+size);	
}

template <typename T>
void packv(std::vector<unsigned char> &packet, T* data, int size)
{
	for(int i=0;i<size;i++)
	{
		pack(packet,data[i]);
	}
}


template <typename T>
T unpack(std::vector<unsigned char> &packet)
{
	T data;
	T* ptr=(T*)packet.data();
	int numBytes=sizeof(T);
	int size=packet.size();
	
	data=ptr[0];
	for(int i=numBytes;i<size;i++)
	{
		packet[i-numBytes]=packet[i];
	}
	packet.resize(size-numBytes);

	return data;
}

template <typename T>
void unpackv(std::vector<unsigned char> &packet, T* data, int size)
{
	for(int i=0;i<size;i++)
	{
		data[i]=unpack<T>(packet);
	}
}

void serializaInvocacion(rpcInvocacion op, std::vector<unsigned char> &packet)
{
	
	//empaqueta tipo de operacion
	unsigned char tipoOp=op.tipoOp;
	
	pack(packet,tipoOp);
	//empaqueta datos
	
	switch(op.tipoOp)
			{

				case opListFiles:
				{
					pack(packet, op.listFiles.none);
				}break;

				case opReadFile:
				{
					pack(packet,op.readFile.fileName);
				}break;

				case opWriteFile:
				{
					pack(packet,op.writeFile.fileName);
					pack(packet,op.writeFile.data);
					pack(packet,op.writeFile.dataLength);
				}break;

				default:
					std::cout<<"Error:  Operación no definida\n";
				break;		
			}
}


rpcInvocacion deserializaInvocacion(std::vector<unsigned char> &packet)
{
	rpcInvocacion op;
	
	//desempaqueta tipo de operacion
	unsigned char tipoOp=unpack<unsigned char>(packet);
	op.tipoOp=(rpcTipoOperacion)tipoOp;
	
	//desempaqueta datos
	
	switch(op.tipoOp)
			{
				case opListFiles:
				{
					unpackv(packet, &op.listFiles.none ,0);
				}break;

				case opReadFile:
				{
					unpackv(packet,&op.readFile.fileName,1);
				}break;

				case opWriteFile:
				{
					unpackv(packet,&op.writeFile.fileName,1);
					unpackv(packet,&op.writeFile.data,1);
					unpackv(packet,&op.writeFile.dataLength,1);
				}break;

				default:
					std::cout<<"Error:  Operación no definida\n";
				break;		
			}
	return op;
};