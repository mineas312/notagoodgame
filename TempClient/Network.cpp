#include "stdafx.h"
#include "Network.h"

void Network::init(const char * ip, Uint16 port)
{
	if (SDLNet_Init() < 0)
	{
#ifdef _DEBUG
		printf("SDLNet_Init Error: %s\n", SDLNet_GetError());
#endif // DEBUG
		fprintf(stderr, "SDLNet_Init Error: %s\n", SDLNet_GetError());
		exit(2);
	}
	socket = SDLNet_UDP_Open(0);
	if (!socket)
	{
#ifdef _DEBUG
		printf("SDLNet_UDP_Open Error: %s\n", SDLNet_GetError());
#endif // DEBUG
		fprintf(stderr, "SDLNet_UDP_Open Error: %s\n", SDLNet_GetError());
		exit(2);
	}
	SDLNet_ResolveHost(&addr, ip, port);

	sendPacket = SDLNet_AllocPacket(1024);
	if (!sendPacket)
	{
#ifdef _DEBUG
		printf("SDLNet_AllocPacket Error: %s\n", SDLNet_GetError());
#endif // _DEBUG
		fprintf(stderr, "SDLNet_AllocPacket Error: %s\n", SDLNet_GetError());
		exit(2);
	}

	recvPacket = SDLNet_AllocPacket(1024);
	if (!recvPacket)
	{
#ifdef _DEBUG
		printf("SDLNet_AllocPacket Error: %s\n", SDLNet_GetError());
#endif // _DEBUG
		fprintf(stderr, "SDLNet_AllocPacket Error: %s\n", SDLNet_GetError());
		exit(2);
	}
}

void Network::send(char * data)
{
	int numsent;
	numsent = SDLNet_UDP_Send(socket, sendPacket->channel, sendPacket);
	if (numsent)
	{
#ifdef _DEBUG
		printf("SDLNet_UDP_Send: %s\n", SDLNet_GetError());
#endif // _DEBUG
		fprintf(stderr, "SDLNet_UDP_Send: %s\n", SDLNet_GetError());
	}
}

void Network::recv()
{
	int numrcv;
	numrcv = SDLNet_UDP_Recv(socket, recvPacket);
	if (numrcv)
	{
#ifdef _DEBUG
		printf("SDLNet_UDP_Recv: %s\n", SDLNet_GetError());
#endif // _DEBUG
		fprintf(stderr, "SDLNet_UDP_Recv: %s\n", SDLNet_GetError());
	}
}

void Network::reallocPacket(UDPpacket * packet, int size)
{
	int newSize;
	newSize = SDLNet_ResizePacket(packet, size);
	if (newSize < size)
	{
#ifdef _DEBUG
		printf("SDLNet_ResizePacket Error: %s\n", SDLNet_GetError());
#endif // _DEBUG
		fprintf(stderr, "SDLNet_ResizePacket Error: %s\n", SDLNet_GetError());
	}
}

void Network::close()
{
	SDLNet_UDP_Close(socket);
	SDLNet_Quit();
}

Network * netptr;