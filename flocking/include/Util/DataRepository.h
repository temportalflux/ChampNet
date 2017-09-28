#pragma once

#include <Trackable.h>

#include <string>
#include <windows.h>
#include <assert.h>
#include <map>

const UINT MAX_STRING_VAL_SIZE = 48;

union DataUnion
{
	int intVal;
	float floatVal;
	double doubleVal;
	char stringVal[MAX_STRING_VAL_SIZE];
	UINT uintVal;

};

enum DataType
{
	INT_VAL,
	FLOAT_VAL,
	DOUBLE_VAL,
	STRING_VAL,
	UINT_VAL
};

class DataEntry: public Trackable
{
public:
	DataEntry( int val );
	DataEntry( float val );
	DataEntry( double val );
	DataEntry( const std::string& val );
	DataEntry( UINT val );
	DataEntry():mType(UINT_VAL){};

	~DataEntry(){};

	inline DataType getType() const { return mType; };
	inline int getIntVal() const { assert( mType == INT_VAL ); return mData.intVal; };
	inline float getFloatVal() const { assert( mType == FLOAT_VAL ); return mData.floatVal; };
	inline double getDoubleVal() const { assert( mType == DOUBLE_VAL ); return mData.doubleVal; };
	inline std::string getStringVal() const { assert( mType == STRING_VAL ); return std::string(mData.stringVal); };
	inline UINT getUIntVal() const { assert( mType == UINT_VAL ); return mData.uintVal; };
private:
	DataType mType;
	DataUnion mData;

};

typedef std::string DataKey;

class DataRepository : public Trackable
{
	typedef std::map<DataKey, DataEntry> Map;

public:
	typedef Map::const_iterator Iterator;

	DataRepository(){};
	~DataRepository(){};

	void addEntry( const DataKey& key, int val );
	void addEntry( const DataKey& key, float val );
	void addEntry( const DataKey& key, double val );
	void addEntry( const DataKey& key, const std::string& val );
	void addEntry( const DataKey& key, UINT val );

	const bool hasEntry(const DataKey& key);
	const DataEntry& getEntry( const DataKey& key );
	const Map::const_iterator begin();
	const Map::const_iterator end();

private:
	Map mMap;
};

extern DataRepository* gpDataRepository;
