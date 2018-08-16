#pragma once
#include <string>

class Treasure
{
public:
	/* CONSTRUCTOR */
	Treasure(std::string TreasureType, unsigned int TreasureValue, std::string TreasureSymbol);

	/* INLINE FUNCTIONS - GETTERS */
	virtual std::string GetType() { return mType; }
	virtual unsigned int GetValue() { return mValue; }
	std::string GetSymbol() { return mSymbol; }
	bool IsTaken() { return mIsTaken; }

	/* INLINE FUNCTIONS - SETTERS */
	void Take() { mIsTaken = true; }
protected:
	/* PROTECTED VARIABLES */
	std::string mType;
	unsigned int mValue;
	std::string mSymbol;
	bool mIsTaken;
};
