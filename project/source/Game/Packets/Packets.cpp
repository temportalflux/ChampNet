#include "Game\Packets\Packet.h"

#include <cmath>

// ONLY LOCAL TO THIS FILE
size_t min(size_t a, size_t b) {
	return a < b ? a : b;
}

void writeToCharData(char *data, std::string &string, size_t maxSizeOfData) {
	size_t length = min(string.length(), maxSizeOfData - 1);
	strncpy(data, string.c_str(), length);
	data[length] = '\0';
}
