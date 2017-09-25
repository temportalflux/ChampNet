
#include <RakNet\RakPeerInterface.h>
#include <string.h>
#include <RakNet\MessageIdentifiers.h>

enum GameMessages {
	ID_GAMEOBJECT = ID_USER_PACKET_ENUM,
};

#pragma pack(push, 1)

class GameObject
{
	// RAW DATA
	//int id; // 4 bytes
	//char name[32]; // (char = 1 byte) * 32 = 32

	// Structure data
	struct DataGameObject {
		int id;
		char name[32];
	} data[1];

public:
	virtual void update() = 0;

	virtual unsigned int write(char *buffer) const
	{
		const char *const start = buffer;

		/*
		// write variable to buffer
		memcpy(buffer, &id, sizeof(id));
		// offset buffer address by size written
		buffer += sizeof(id);
		// repeat as needed

		//memcpy(buffer, &name, sizeof(name));
		buffer += sizeof(name);
		//*/

		memcpy(buffer, &data, sizeof(data));
		buffer += sizeof(data);

		return buffer - start;
	}

};

class Character : public GameObject {
	
	//float posX, posY, posZ; // (float = 4 bytes) * 3 floats = 12 bytes
	//int items[32]; // (int = 4bytes) * 32 = 128 bytes

	struct DataCharacter {
		float posX, posY, posZ;
		int items[32];
	} data[1];

public:
	virtual void update() {}

	virtual unsigned int write(char *buffer) const
	{
		const char *const start = buffer;

		// Write super information
		buffer += GameObject::write(buffer);

		/*
		memcpy(buffer, &posX, sizeof(posX));
		buffer += sizeof(posX);

		memcpy(buffer, &posX, sizeof(posX));
		buffer += sizeof(posX);

		memcpy(buffer, &posY, sizeof(posY));
		buffer += sizeof(posY);

		memcpy(buffer, &posZ, sizeof(posZ));
		buffer += sizeof(posZ);

		memcpy(buffer, &items, sizeof(items));
		buffer += sizeof(items);
		//*/

		memcpy(buffer, &data, sizeof(data));
		buffer += sizeof(data);

		return buffer - start;
	}

};

#pragma pack(pop)

void updateNetworking() {
	// .. skipping to the part where we need to send things

	// tmp data that needs to be sent
	Character tmpCharacter;

	sizeof(tmpCharacter); // 176 bytes in size
	// 176 = id=4 + name=32 + (posX+posY+posZ)=12 + items=128

	char sendBuff[2048]; // WILL run out of space, eventually...
	unsigned int bytesWritten = 0;

	// Package the packet ID
	sendBuff[0] = ID_GAMEOBJECT;
	bytesWritten++;

	// Write all packet data
	bytesWritten += tmpCharacter.write(sendBuff + bytesWritten); // ensure offset for packet ID

	// dummy send function (assuming all data has been serialized

	// this is a part of the networking system
	RakNet::RakPeerInterface *peer;

	//peer->Send(sendBuff, bytesWritten, ...)

}

int main() {

	// test buffer stuff
	updateNetworking();

	return 0;
}
