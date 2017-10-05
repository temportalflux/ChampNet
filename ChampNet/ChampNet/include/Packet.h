#ifndef _CHAMPNET_PACKET_H
#define _CHAMPNET_PACKET_H

#include <string>

namespace ChampNet
{

	class Packet
	{
		friend class PacketQueue;

	private:
		unsigned int mAddressLength;
		char* mAddress;
		unsigned int mDataLength;
		unsigned char* mData;

		Packet* mNext;

	public:
		Packet(const unsigned int lengthAddress, char* address, const unsigned int dataLength, const unsigned char* data);
		~Packet();

		void copy(const unsigned char* source, unsigned char* dest, unsigned int length);

	};

	class PacketQueue
	{

	private:
		Packet* mHead;
		Packet* mTail;

	public:
		PacketQueue();
		~PacketQueue();

		// Pushes a packet onto the end of the list
		void enqueue(Packet* packet);

		// Pops a packet from the front of the list
		// Parameter will be set with the first packet
		void dequeue(Packet *&packet);

		// Deallocates all packets
		void clear();

		bool isEmpty();

	};

}

#endif // _CHAMPNET_PACKET_H