#pragma once

#include <Trackable.h>
#include <string>

class DataRepository;
namespace json { class JSON; };

struct DataPoint : public Trackable {
	friend class GameData;

private:
	json::JSON* mJson;

	DataPoint(json::JSON* json);

public:
	DataPoint();
	DataPoint operator[](const std::string& str);

	std::string asString();
	int asInt();
	double asDouble();
	bool asBool();

};

class GameData : public Trackable {

private:
	std::string mFilePath;
	//DataRepository* mData;
	json::JSON* mData;

	//void addData(std::string previous, std::string key, json::JSON* value);
	const std::string generateTabs(unsigned int tabs, char tabChar);

public:

	GameData(const std::string& filePath);
	virtual ~GameData();

	void load();
	void save();

	bool getJson(DataPoint& out);
	const std::string dump(const std::string& key, json::JSON* value, unsigned int tabs = 0, char tabChar = '\t');

};
