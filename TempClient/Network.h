#pragma once
#include <SDL_net.h>

enum PacketType
{
	ENTITY_POSITION,
	TOTAL_PACKETTYPES
};

class Network
{
public:
	Network() : sendPacket{ NULL }, recvPacket{ NULL }
	{}

	void init(const char * ip, Uint16 port);

	void close();

private:
	void send(char * data);

	void recv();

	void reallocPacket(UDPpacket * packet, int size);

private:
	UDPsocket socket;
	IPaddress addr;
	UDPpacket * sendPacket;
	UDPpacket * recvPacket;
};

extern Network * netptr;