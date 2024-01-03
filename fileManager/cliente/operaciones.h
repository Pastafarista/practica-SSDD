// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS  2023/2024

# pragma once
# include <vector>

typedef enum{
	opEndConnection=0,
	opListFiles=1,
	opReadFile=2,
	opWriteFile=3,
}tipoOperacion;

typedef enum{
	opConnectServer=0,
	opConnectClient=1,
	opDisconnectServer=2
}opBroker;

typedef enum{
	tipoFilemanager=0,
	tipoMultmatrix=1,
}tipoServidor;

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
