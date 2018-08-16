#pragma once
#include "Treasure.h"
#include <string>
#include <vector>
#include <memory>

class TreasureGroup :
	public Treasure
{
public:
	/* CONSTRUCTOR */
	TreasureGroup(std::string TreasureSymbol, std::vector<std::shared_ptr<Treasure>> DroppedTreasures);

	/* FUNCTION OVERRIDES */
	std::string GetType() { return std::string("a group of minerals"); }
	unsigned int GetValue();
private:
	std::vector<std::shared_ptr<Treasure>> mDroppedTreasure;
};

