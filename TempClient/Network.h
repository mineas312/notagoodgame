#pragma once
#include <SDL_net.h>

#define TICKRATE 1000/32

enum PacketType
{
	ENTITY_POSITION,
	CONNECT,
	DISCONNECT,
	ESTABILISH_CONN,
	TOTAL_PACKETTYPES
};

class Network
{
public:
	Network() : sendPacket{ NULL }, recvPacket{ NULL }, id { -1 }
	{}

	void init(const char * ip, Uint16 port);

	void close();

	void networkUpdate(int charX, int charY);

	void joinServer();

	void send(const char * data, int len, PacketType type);

	void recv();

private:

	void processPacket();

	void reallocPacket(UDPpacket * packet, int size);

	void Uint8ToInt(Uint8 * src, int & dest);

	void intToUint8(int src, Uint8 * dst);

private:
	int id;
	UDPsocket socket;
	IPaddress addr;
	UDPpacket * sendPacket;
	UDPpacket * recvPacket;
};

extern Network * netptr;