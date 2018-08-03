#pragma once
#include "Client.h"
#include <cstdio>
#include <vector>

#define SERVER_SLOTS 100

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
	Network() : nextSlot{ 0 }
	{}

	void init(Uint16 port);

	void close();

	void send(Client client, int len, const char * data, PacketType type);

	void recv();

	void addClient();

	void disconnectClient(int id);

private:
	void reallocPacket(UDPpacket * packet, int size);

	void processPacket();

	void Uint8ToInt(Uint8 * src, int & dest);

	void intToUint8(int src, Uint8 * dst);

public:
	Client * clients;
	int nextSlot;
private:
	UDPsocket socket;
	UDPpacket * sendPacket;
	UDPpacket * recvPacket;
	IPaddress addr;
};

extern Network * netptr;