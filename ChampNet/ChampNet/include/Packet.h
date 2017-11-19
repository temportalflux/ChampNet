#ifndef _CHAMPNET_PACKET_H
#define _CHAMPNET_PACKET_H

#include <string>
#include <RakNet\RakNetTime.h>

namespace ChampNet
{

	struct TimeStamp
	{
		// If the timestamps were found in the packet
		bool timesLoaded = false;
		// The lcoal time that the packet was read at
		RakNet::Time packetReadTime_local = 0;
		// The local time duration the packet took to start reading
		RakNet::Time readDiff_local = 0;
		// The remote time that the packet was sent at
		RakNet::Time sentTime_remote = 0;
		// The total time (local) that the packet took to be received/read
		RakNet::Time totalTransferTime_local = 0;
	};

	/** Author: Dustin Yost
	 * A class to handle packet data from RakNet
	 */
	class Packet
	{
		friend class PacketQueue;

	private:
		// The character length of the address
		unsigned int mAddressLength;
		// A pointer to the characters of the address
		const char* mAddress;
		// The length of bytes in the data
		unsigned int mDataLength;
		// The dynamic array of packet data
		unsigned char* mData;

		// The next packet in the PacketQueue
		Packet* mNext;

	public:

		TimeStamp timestampInfo;

		Packet(const unsigned int lengthAddress, const char* address, const unsigned int dataLength, const unsigned char* data);
		~Packet();

		// Create a new char[] at dest, and copies the contents of the range [0, length) from source into dest
		void copy(const unsigned char* &source, unsigned char* &dest, unsigned int length);

		// Returns the ID of this packet (assumed the first byte of data in the packet)
		inline unsigned int getID() const { return this->mData[0]; }

		// Sets address and length to the address characters of the ip address
		void getAddress(const char* &address, unsigned int &length);
		// Returns the const char* address as a string (wraps getAddress(const char*, unsigned int))
		std::string getAddress();

		// Sets data and length to the byte data from the packet
		void getData(unsigned char* &data, unsigned int &length);
		// Returns the data length (same thing as length in getData)
		inline unsigned int getDataLength() const { return mDataLength; }

		// Casts the packet data to some structure T, returning the length of data as a reference parameter
		template <typename T>
		T* getPacketAs(unsigned int &dataSize)
		{
			unsigned char *data;
			this->getData(data, dataSize);
			return (T*)data;
		}

	};

	/** Author: Dustin Yost
	* A class to handle a queue of Packets (wrapped RakNet packets)
	*/
	class PacketQueue
	{

	private:
		Packet* mHead;
		Packet* mTail;
		int mCount;

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

		// Returns true if there are no packets in the queue (count == 0)
		bool isEmpty();

		// Returns the quantity of packets in the queue
		inline int getCount() const { return mCount; };

	};

}

#endif // _CHAMPNET_PACKET_H