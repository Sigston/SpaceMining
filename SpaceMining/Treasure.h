#pragma once
#include <string>

class Treasure
{
public:
	Treasure(std::string TreasureType, unsigned int TreasureValue);

	std::string GetType() { return mType; }
	unsigned int GetValue() { return mValue; }
	char GetChar() { return mChar; }
private:
	std::string mType;
	unsigned int mValue;
	char mChar;
};

