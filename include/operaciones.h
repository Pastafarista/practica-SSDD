#pragma once

typedef enum{
	
	opSumaDouble=1,
	opSumaFloat=2,
	opSumaInt=3,
	opSumaVector4Int=4
}rpcTipoOperacion;

typedef enum personaOp{
	
	constructorOp=1,
	setDniOp=2,setNombreOp=3,setEdadOp=4,
	getDniOp=5,getNombreOp=6,getEdadOp=7,
	saveOp=8,loadOp=9,
	destructorOp=10
}personaOp;



typedef struct __attribute__ ((packed)){
	rpcTipoOperacion tipoOp;
	union{
		struct{
			int dato1;
			int dato2;
		}sumaInt;	

		struct{
			float dato1;
			float dato2;
		}sumaFloat;	

		struct{
			double dato1;
			double dato2;
		}sumaDouble;
		
		struct{
			int dato1[4];
			int dato2[4];
		}sumaVector4Int;
		
	};
}rpcInvocacion;


typedef struct __attribute__ ((packed)){
	rpcTipoOperacion tipoOp;
	union{
		struct{
			int res;
		}sumaInt;	

		struct{
			float res;
		}sumaFloat;	

		struct{
			double res;
		}sumaDouble;
		
		struct{
			int res[4];
		}sumaVector4Int;

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
				case opSumaInt:
				{
					pack(packet,op.sumaInt.dato1);
					pack(packet,op.sumaInt.dato2);
				}break;
				
				case opSumaFloat:
				{
					pack(packet,op.sumaFloat.dato1);
					pack(packet,op.sumaFloat.dato2);
				}break;
			
				case opSumaDouble:
				{
					pack(packet,op.sumaDouble.dato1);
					pack(packet,op.sumaDouble.dato2);
				}break;
			
				case opSumaVector4Int:
				{
					packv(packet,op.sumaVector4Int.dato1,4);
					packv(packet,op.sumaVector4Int.dato2,4);
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
				case opSumaInt:
				{
					unpackv(packet,&op.sumaInt.dato1,1);
					unpackv(packet,&op.sumaInt.dato2,1);
				}break;
				
				case opSumaFloat:
				{
					unpackv(packet,&op.sumaFloat.dato1,1);
					unpackv(packet,&op.sumaFloat.dato2,1);
				}break;
			
				case opSumaDouble:
				{
					unpackv(packet,&op.sumaDouble.dato1,1);
					unpackv(packet,&op.sumaDouble.dato2,1);
				}break;
			
				case opSumaVector4Int:
				{
					unpackv(packet,op.sumaVector4Int.dato1,4);
					unpackv(packet,op.sumaVector4Int.dato2,4);
				}break;
				default:
					std::cout<<"Error:  Operación no definida\n";
				break;		
			}
	return op;
};