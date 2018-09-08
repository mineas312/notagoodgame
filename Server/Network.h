#pragma once
#include "Client.h"

#define SERVER_SLOTS 100

enum PacketType
{
	ENTITY_POSITION,
	CONNECT,
	DISCONNECT,
	SEND_CLIENTS,
	ESTABILISH_CONN,
	ENTITY_DISCONNECT,
	TOTAL_PACKETTYPES
};

class Network
{
public:
	Network() : clients(nullptr), nextSlot{0}, socket(nullptr), sendPacket(nullptr), recvPacket(nullptr), addr()
	{
	}

	void init(Uint16 port);

	void close() const;

	void send(Client client, int len, const char * data, PacketType type) const;

	void recv();

	void addClient();

	void disconnectClient(int id);

	void Uint8ToInt(Uint8* __restrict src, int & dest) const noexcept;

	void intToUint8(int src, Uint8* __restrict dst) const noexcept;

private:
	void reallocPacket(UDPpacket * packet, int size) const;

	void processPacket();

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