#include "TreasureGroup.h"

/* CONSTRUCTOR */

// Takes a TreasureSymbol and a vector of Treasure pointers for dropped treasures. Calls the default constructor.
TreasureGroup::TreasureGroup(std::string TreasureSymbol, std::vector<std::shared_ptr<Treasure>> DroppedTreasures) : 
	Treasure("Group", 0, TreasureSymbol), mDroppedTreasure(DroppedTreasures)
{
}

/* FUNCTION OVERRIDES */

// Overrides GetValue() to return a sum of the values of the vector of dropped treasures.
unsigned int TreasureGroup::GetValue()
{
	unsigned int Sum = 0;
	for (auto it = mDroppedTreasure.begin(); it < mDroppedTreasure.end(); ++it)
	{
		Sum += (*it)->GetValue();
	}
	return(Sum);
}
