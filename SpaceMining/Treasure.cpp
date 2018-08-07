#include "Treasure.h"

Treasure::Treasure(std::string TreasureType, unsigned int TreasureValue) :
	mType(TreasureType), mValue(TreasureValue)
{
	if (TreasureType == "Coppernium")
	{
		mChar = '@';
	}
	else if (TreasureType == "Bronzium")
	{
		mChar = '#';
	}
	else if (TreasureType == "Silverium")
	{
		mChar = '$';
	}
	else if (TreasureType == "Goldium")
	{
		mChar = 156;
	}
}
