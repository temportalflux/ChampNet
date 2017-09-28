#include "Util\DataRepository.h"

DataEntry::DataEntry(int val) {
	mType = DataType::INT_VAL;
	mData = DataUnion();
	mData.intVal = val;
}

DataEntry::DataEntry(float val) {
	mType = DataType::FLOAT_VAL;
	mData = DataUnion();
	mData.floatVal = val;
}

DataEntry::DataEntry(double val) {
	mType = DataType::DOUBLE_VAL;
	mData = DataUnion();
	mData.doubleVal = val;
}

DataEntry::DataEntry(const std::string& val) {
	mType = DataType::INT_VAL;
	mData = DataUnion();
	strncpy_s(mData.stringVal, val.c_str(), val.length());
}

DataEntry::DataEntry(UINT val) {
	mType = DataType::UINT_VAL;
	mData = DataUnion();
	mData.uintVal = val;
}


void DataRepository::addEntry(const DataKey& key, int val) {
	mMap[key] = DataEntry(val);
}

void DataRepository::addEntry(const DataKey& key, float val) {
	mMap[key] = DataEntry(val);
}

void DataRepository::addEntry(const DataKey& key, double val) {
	mMap[key] = DataEntry(val);
}

void DataRepository::addEntry(const DataKey& key, const std::string& val) {
	mMap[key] = DataEntry(val);
}

void DataRepository::addEntry(const DataKey& key, UINT val) {
	mMap[key] = DataEntry(val);
}

const bool DataRepository::hasEntry(const DataKey& key) {
	return mMap.count(key) == 1;
}

const DataEntry& DataRepository::getEntry(const DataKey& key) {
	assert(this->hasEntry(key));
	return mMap[key];
}

const DataRepository::Map::const_iterator DataRepository::begin() {
	return mMap.begin();
}

const DataRepository::Map::const_iterator DataRepository::end() {
	return mMap.end();
}
