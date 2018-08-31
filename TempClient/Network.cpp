#include "stdafx.h"
#include "Network.h"

void Network::init(const char * ip, Uint16 port, std::vector<Entity> * ent)
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
	entities = ent;
}

void Network::send(const char * data, int len, PacketType type)
{
	if (id == -1 && type != CONNECT)
		return;

	Uint8 typeUint8[4];
	intToUint8(type, typeUint8);

	Uint8 uid[4];
	intToUint8(id, uid);

	Uint8 * msg = new Uint8[8 + len];
	memcpy(msg, typeUint8, 4);
	memcpy(&msg[4], uid, 4);
	memcpy(&msg[8], data, len);

	sendPacket->len = 8 + len;
	sendPacket->data = msg;
	sendPacket->address = addr;

	int numsent;
	numsent = SDLNet_UDP_Send(socket, -1, sendPacket);
	if (!numsent)
	{
#ifdef _DEBUG
		printf("SDLNet_UDP_Send: %s\n", SDLNet_GetError());
#endif // _DEBUG
		fprintf(stderr, "SDLNet_UDP_Send: %s\n", SDLNet_GetError());
	}
	delete[] msg;
}

void Network::recv()
{
	int numrcv;
	numrcv = SDLNet_UDP_Recv(socket, recvPacket);
	if (numrcv)
	{
		processPacket();
	}
}

void Network::disconnect()
{
	send("", 0, DISCONNECT);
}

void Network::processPacket()
{
	int type;
	Uint8ToInt(&recvPacket->data[0], type);
	switch ((PacketType)type)
	{
	case ENTITY_DISCONNECT:
		int _id;
		netptr->Uint8ToInt(&recvPacket->data[4], _id);
		disconnectEntity(_id);
		break;
	case SEND_CLIENTS:
	{
		int eid, ex, ey;
		netptr->Uint8ToInt(&recvPacket->data[4], ex);
		netptr->Uint8ToInt(&recvPacket->data[8], ey);
		netptr->Uint8ToInt(&recvPacket->data[12], eid);
		Entity en;
		en.id = eid;
		en.box.x = ex;
		en.box.y = ey;
		entities->push_back(en);
		break;
	}
	case CONNECT:
		Uint8ToInt(&recvPacket->data[4], id);
		break;
	case ENTITY_POSITION:
	{
		int eid, ex, ey;
		netptr->Uint8ToInt(&recvPacket->data[4], ex);
		netptr->Uint8ToInt(&recvPacket->data[8], ey);
		netptr->Uint8ToInt(&recvPacket->data[12], eid);
		for (Entity & e : *entities)
		{
			if (e.id == eid)
			{
				e.moveInterpolated(ex, ey);
			}
		}
		break;
	}
	case DISCONNECT:
		printf("Disconnected from server");
		id = -1;
		break;
	default:
		printf("Unrecognized packetType: %d\n", type);
		break;
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

void Network::networkUpdate(int charX, int charY)
{
	//TODO BUG HERE
	static Uint32 start = SDL_GetTicks();

	Uint32 delta = SDL_GetTicks();

	recv();

	while (delta - start >= TICKRATE)
	{
		Uint8 tmp[8];
		intToUint8(charX, &tmp[0]);
		intToUint8(charY, &tmp[4]);
		send((char*)tmp, 8, ENTITY_POSITION);
		start += TICKRATE;
	}
	delta = SDL_GetTicks();
}

void Network::joinServer()
{
	send("", 0, CONNECT);
}

void Network::Uint8ToInt(Uint8* __restrict src, int & dest) noexcept
{
	dest = (src[0] << 24) | (src[1] << 16) | (src[2] << 8) | (src[3]);
}

void Network::intToUint8(int src, Uint8* __restrict dst) noexcept
{
	Uint8 tmp[4];
	for (int i = 0; i < 4; i++)
		tmp[3 - i] = (src >> (i * 8));
	std::memcpy(dst, tmp, 4);
}

void Network::disconnectEntity(int _id)
{
	for (Entity & e : *entities)
	{
		if (e.id == _id)
		{
			entities->erase(std::remove_if(entities->begin(), entities->end(), [&](Entity & a) {return a.id == e.id; }));
		}
	}
}

Network * netptr;