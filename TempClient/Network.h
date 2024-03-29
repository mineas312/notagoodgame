#pragma once
#include <SDL_net.h>
#include "Map.h"
#include "Entity.h"

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
	Network() : sendPacket{NULL}, recvPacket{NULL}, entities(nullptr), id {-1}, socket(nullptr), addr()
	{
	}

	void init(const char * ip, Uint16 port, std::vector<Entity> * ent);

	void close() const;

	void networkUpdate(int charX, int charY);

	void joinServer();

	void send(const char * data, int len, PacketType type);

	void recv();

	void disconnect();

private:

	void processPacket();

	void reallocPacket(UDPpacket * packet, int size) const;

	void Uint8ToInt(Uint8* __restrict src, int & dest) const noexcept;

	void intToUint8(int src, Uint8* __restrict dst) const noexcept;

	void disconnectEntity(int id);

private:
	int id;
	UDPsocket socket;
	IPaddress addr;
	UDPpacket * sendPacket;
	UDPpacket * recvPacket;
	std::vector<Entity> * entities;
};

extern Network * netptr;