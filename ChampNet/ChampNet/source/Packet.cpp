#include "Packet.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

namespace ChampNet
{

	Packet::Packet(const unsigned int lengthAddress, char* address, const unsigned int dataLength, const unsigned char* data)
	{
		mAddressLength = lengthAddress;
		mAddress = address;
		copy(data, mData, dataLength);
	}

	Packet::~Packet()
	{
		mAddressLength = 0;
		mDataLength = 0;
		mAddress = NULL;
		delete[] mData;
		mData = NULL;
	}

	void Packet::copy(const unsigned char* source, unsigned char* dest, unsigned int length)
	{
		dest = new unsigned char[length];
		for (unsigned int i = 0; i < length; i++)
		{
			dest[i] = source[i];
		}
	}

	void Packet::getAddress(char* &address, unsigned int &length)
	{
		address = this->mAddress;
		length = this->mAddressLength;
	}

	void Packet::getData(unsigned char* &data, unsigned int &length)
	{
		data = this->mData;
		length = this->mDataLength;
	}

	PacketQueue::PacketQueue()
	{
		mHead = NULL;
		mTail = NULL;
	}

	PacketQueue::~PacketQueue()
	{
		this->clear();
	}

	void PacketQueue::enqueue(Packet* data)
	{
		// Add node to end of list

		// If list empty, make the first node
		if (mHead == NULL)
		{
			// Set to both beginning and end
			mHead = data;
			mTail = data;
		}
		else
		{
			// Not the first, add to last node and set as last
			mTail->mNext = data;
			mTail = data;
		}

		mCount++;

	}

	void PacketQueue::dequeue(Packet *&data)
	{
		// Always ensure data is wiped on entrance
		data = NULL;

		// Check if the list is empty
		if (!isEmpty())
		{
			// Get the first element in the queue
			data = mHead;

			// Set the first element to the next element
			mHead = mHead->mNext;

			// Set the next element of the previous start to NULL (no longer a part of the list)
			data->mNext = NULL;

			// If the head is now empty, make sure to empty the tail
			if (mHead == NULL)
			{
				mTail = NULL;
			}

			mCount--;

		}

	}

	void PacketQueue::clear()
	{
		Packet* data;
		while (!isEmpty())
		{
			dequeue(data);
			delete data;
		}
	}

	bool PacketQueue::isEmpty()
	{
		return mHead == NULL;
	}

}
