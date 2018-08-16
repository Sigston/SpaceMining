#include "Treasure.h"

/* CONSTRUCTOR */

// Fills the varaibles for the given treasure.
Treasure::Treasure(std::string TreasureType, unsigned int TreasureValue, std::string TreasureSymbol) :
	mType(TreasureType), mValue(TreasureValue), mSymbol(TreasureSymbol), mIsTaken(false)
{
}
