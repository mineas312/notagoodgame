#include "Network.h"

void Network::init(Uint16 port)
{
	if (SDLNet_Init() == -1)
	{
		printf("[ERROR] SDLNet_Init Error: %s\n", SDLNet_GetError());
		exit(2);
	}
	socket = SDLNet_UDP_Open(port);
	if (!socket)
	{
		printf("[ERROR] SDLNet_UDP_Open Error: %s\n", SDLNet_GetError());
		exit(2);
	}
	if (SDLNet_ResolveHost(&addr, NULL, port) == -1)
	{
		printf("[ERROR] SDLNet_ResolveHost Error: %s\n", SDLNet_GetError());
		exit(2);
	}
	sendPacket = SDLNet_AllocPacket(1024);
	if (!sendPacket)
	{
		printf("[ERROR] SDLNet_AllocPacket Error: %s\n", SDLNet_GetError());
		exit(2);
	}

	recvPacket = SDLNet_AllocPacket(1024);
	if (!recvPacket)
	{
		printf("[ERROR] SDLNet_AllocPacket Error: %s\n", SDLNet_GetError());
		exit(2);
	}

	printf("[INFO] Network initialization succeed\n");
	clients = new Client[SERVER_SLOTS];
}

void Network::send(Client client, int len, const char * data, PacketType type)
{
	sendPacket->address = client.addr;
	sendPacket->len = 4 + len;

	Uint8 * tmp = new Uint8[4 + len];

	Uint8 utype[4];
	intToUint8(type, utype);

	memcpy(&tmp[0], utype, 4);
	memcpy(&tmp[4], data, len);
	sendPacket->data = tmp;

	int numsent;
	numsent = SDLNet_UDP_Send(socket, -1, sendPacket);
	if (!numsent)
	{
		printf("[ERROR] SDLNet_UDP_Send: %s\n", SDLNet_GetError());
	}
	delete[] tmp;
}

void Network::recv()
{
	int numrcv;
	numrcv = SDLNet_UDP_Recv(socket, recvPacket);
	if (numrcv)
		processPacket();
}

void Network::addClient()
{
	if (clients[nextSlot].used)
		printf("[Error] Tried to write a new client to a used slot: %d !\n", nextSlot);
	else
	{
		clients[nextSlot].used = true;
		clients[nextSlot].addr = recvPacket->address;
		clients[nextSlot].entity.setPosition(0, 0);
		Uint8 uint8id[4];
		intToUint8(nextSlot, &uint8id[0]);
		send(clients[nextSlot], 4, (char*)uint8id, CONNECT);
		for (int i = 0; i < SERVER_SLOTS; i++)
		{
			if (i == nextSlot || !clients[i].used)
				continue;
			Uint8 data[12];
			netptr->intToUint8(netptr->clients[i].entity.x, &data[0]);
			netptr->intToUint8(netptr->clients[i].entity.y, &data[4]);
			netptr->intToUint8(i, &data[8]);
			send(clients[nextSlot], 12, (char*)data, SEND_CLIENTS);

			Uint8 data2[12];
			netptr->intToUint8(netptr->clients[nextSlot].entity.x, &data2[0]);
			netptr->intToUint8(netptr->clients[nextSlot].entity.y, &data2[4]);
			netptr->intToUint8(nextSlot, &data2[8]);
			send(clients[i], 12, (char*)data2, SEND_CLIENTS);
		}
	}
	printf("[INFO] Client with ip %d has connected on slot: %d.\n", recvPacket->address.host, nextSlot);
	while (clients[nextSlot].used)
		nextSlot++;
}

void Network::disconnectClient(int id)
{
	clients[id].used = false;
	clients[id].timeout = 0;
	send(clients[id], 0, "", DISCONNECT);
	for (int i = 0; i < SERVER_SLOTS; i++)
	{
		if (i == id)
			continue;
		Uint8 uid[4];
		intToUint8(id, uid);
		send(clients[i], 4, (char*)uid, ENTITY_DISCONNECT);
	}
	printf("[INFO] Client with ID: %d has disconnected!\n", id);
	if(nextSlot > id)
		nextSlot = id;
}

void Network::reallocPacket(UDPpacket * packet, int size)
{
	int newSize;
	newSize = SDLNet_ResizePacket(packet, size);
	if (newSize < size)
	{
#ifdef _DEBUG
		printf("[ERROR] SDLNet_ResizePacket Error: %s\n", SDLNet_GetError());
#endif // _DEBUG
		fprintf(stderr, "[ERROR] SDLNet_ResizePacket Error: %s\n", SDLNet_GetError());
	}
}

void Network::processPacket()
{
	int type, id;
	Uint8ToInt(&recvPacket->data[0], type);
	Uint8ToInt(&recvPacket->data[4], id);

	switch ((PacketType)type)
	{
	case CONNECT:
		addClient();
		break;
	case DISCONNECT:
		disconnectClient(id);
		break;
	case ENTITY_POSITION:
	{
		if (id >= SERVER_SLOTS || id < 0)
		{
			printf("[CLIENT-SIDE ERROR] Given id: %d by a client is invalid !\n", id);
			return;
		}
		else if (clients[id].used)
		{
			int x, y;
			Uint8ToInt(&recvPacket->data[8], x);
			Uint8ToInt(&recvPacket->data[12], y);
			clients[id].entity.x = x;
			clients[id].entity.y = y;
		}
		break;
	}
	case ESTABILISH_CONN:
		if (id >= SERVER_SLOTS || id < 0)
		{
			printf("[CLIENT-SIDE ERROR] Given id: %d by a client is invalid !\n", id);
			return;
		}
		else if (clients[id].used)
			clients[id].timeout = 0;
		break;
	default:
		printf("[ERROR] Unrecognized packetType: %d\n", type);
		break;
	}
}

void Network::Uint8ToInt(Uint8* __restrict src, int & dest) noexcept
{
	dest = (src[0] << 24) | (src[1] << 16) | (src[2] << 8) | (src[3]);
}

void Network::intToUint8(int src, Uint8* __restrict dst) noexcept
{
	Uint8 * tmp = new Uint8[4];
	for (int i = 0; i < 4; i++)
		tmp[3 - i] = (src >> (i * 8));
	std::memcpy(dst, tmp, 4);
}

void Network::close()
{
	SDLNet_UDP_Close(socket);
	SDLNet_Quit();
}

Network * netptr;