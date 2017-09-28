#include "Data\GameData.h"

#include <fstream>
#include <sstream>
#include <json.hpp>

#include "Util\DataRepository.h"

using namespace std;
using json::JSON;

DataPoint::DataPoint(json::JSON* json) : mJson(json) {}
DataPoint::DataPoint() : DataPoint(NULL) {}

DataPoint DataPoint::operator[](const string& str) {
	return DataPoint(&(mJson->at(str)));
}

string DataPoint::asString() { return mJson->ToString(); }
int DataPoint::asInt() {
	switch (mJson->JSONType()) {
		case json::JSON::Class::Floating:
			return (int)mJson->ToFloat();
		case json::JSON::Class::Integral:
			return mJson->ToInt();
		default:
			return 0;
	}
}
double DataPoint::asDouble() {
	switch (mJson->JSONType()) {
		case json::JSON::Class::Floating:
			return mJson->ToFloat();
		case json::JSON::Class::Integral:
			return (double)mJson->ToInt();
		default:
			return 0;
	}
}
bool DataPoint::asBool() { return mJson->ToBool(); }

GameData::GameData(const string& filePath) {
	mFilePath = filePath;
	//mData = new DataRepository();
	mData = NULL;
}

GameData::~GameData() {
	//delete mData;
	if (mData != NULL) {
		delete mData;
		mData = NULL;
	}
}

void GameData::load() {
	if (mData != NULL) {
		delete mData;
		mData = NULL;
	}

	ifstream file(mFilePath);

	if (file.is_open()) {
		//mpDataFile = new FileTracker("data/data.json");
		//mpDataFile->loadFile();

		// https://github.com/MJPA/SimpleJSON/blob/master/src/demo/example.cpp
		//JSON::Parse()

		std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

		JSON value = JSON::Load(content);
		if (value.IsNull()) {
			printf("%s%s\n", "Error loading json from ", mFilePath.c_str());
		}
		else if (value.JSONType() != JSON::Class::Object) {
			printf("%s%s%s\n", "Error root of ", mFilePath.c_str(), " is not an object");
		}
		else {
			JSON::Class type = value.JSONType();
			//this->addData("", "", &value);
			// TODO: value doesnt exist outside this scope
			//mData = &value;
			//memcpy(mData, &value, sizeof(value));
			mData = new JSON(value);
		}

	}

	file.close();
}

/*
void GameData::addData(string previous, string key, JSON* value) {
	string parentKey = (previous.length() > 0 ? previous + "." : "") + key;
	switch (value->JSONType()) {
		case JSON::Class::Object:
			for (std::pair<const std::string, json::JSON> &entry : value->ObjectRange()) {
				this->addData(parentKey, entry.first, &entry.second);
			}
			break;
		case JSON::Class::Array:
			for (int i = 0; i < value->length(); i++) {
				this->addData(parentKey, "" + i, &value[i]);
			}
			break;
		case JSON::Class::Boolean:
			mData->addEntry(DataKey(parentKey.c_str()), value->ToBool());
			break;
		case JSON::Class::Floating:
			mData->addEntry(DataKey(parentKey.c_str()), value->ToFloat());
			break;
		case JSON::Class::Integral:
			mData->addEntry(DataKey(parentKey.c_str()), value->ToInt());
			break;
		case JSON::Class::String:
			mData->addEntry(DataKey(parentKey.c_str()), value->ToString());
			break;
		default:
			break;
	}
}
//*/

void GameData::save() {
	ofstream file(mFilePath, ios::out);

	std::string dump = this->dump("", mData);
	file << dump;

	file.close();
}

const string GameData::dump(const string& key, JSON* value, unsigned int tabs, char tabChar) {
	std::stringstream out;
	
	std::string tabStr = this->generateTabs(tabs, tabChar);
	
	out << tabStr;
	if (!key.empty()) {
		out << '"' << key << '"' << ": ";
	}
	
	if (value == NULL) value = mData;

	JSON::Class type = value->JSONType();
	switch (type) {
		case JSON::Class::Object:
			{
				auto iterBounds = value->ObjectRange();
				out << '{' << '\n';
				for (auto iter = iterBounds.begin(); iter != iterBounds.end();) {
					out << this->dump(iter->first, &(iter->second), tabs + 1, tabChar);

					// increment iterator for next round
					iter++;

					// if the entry after the current iteration is not the end, then a comma is required
					if (iter != iterBounds.end()) {
						out << ",";
					}

					// Always output a new line
					out << '\n';
				}
				out << tabStr << '}';
			}
			break;
		case JSON::Class::Array:
			{
				out << tabStr << '[';
				int max = value->length();
				for (int i = 0; i < max; i++) {
					out << tabStr << this->dump(string(), &value[i], tabs + 1, tabChar);
					if (i != max - 1) {
						out << ",";
					}
					out << '\n';
				}
				out << tabStr << ']';
			}
			break;
		case JSON::Class::Boolean:
			out << (value->ToBool() ? "true" : "false");
			break;
		case JSON::Class::Floating:
			out << value->ToFloat();
			break;
		case JSON::Class::Integral:
			out << value->ToInt();
			break;
		case JSON::Class::String:
			out << '"' << value->ToString() << '"';
			break;
		default:
			//std::cout << value->JSONType << '\n';
			break;
	}
	return out.str();
}

const std::string GameData::generateTabs(unsigned int tabs, char tabChar) {
	std::stringstream out;
	for (unsigned int i = 0; i < tabs; i++) out << tabChar;
	return out.str();
}

bool GameData::getJson(DataPoint& out) {
	if (mData != NULL) {
		out = DataPoint(mData);
	}
	return mData != NULL;
}
