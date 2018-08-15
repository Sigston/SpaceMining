#pragma once
#include <vector>
#include <memory>
#include "Treasure.h"

class Player
{
public:
	/* CONSTRUCTOR */
	Player(char PlayerChar);

	/* PUBLIC FUNCTIONS - MOVEMENT */
	int MoveResult(int Movement);
	void Move(int Movement);

	/* PUBLIC FUNCTIONS - TREASURE */
	void AddTreasure(std::shared_ptr<Treasure> Treasure);
	void DropTreasures();
	bool HasTreasure();
	std::string PrintTreasures();

	/* PUBLIC FUNCTIONS - SCORE */
	void UpdateScore();

	/* INLINE FUNCTIONS - GETTERS */
	int GetScore() { return(mScore); }
	int GetDepth() { return(mDepth); }
	bool IsGoingDown() { return(mGoingDown); }
	bool IsBack() { return(mIsBack); }
	char GetChar() { return(mPlayerChar); }
	int GetTreasureNum() { return mTreasureList.size(); }

	/* INLINE FUNCTIONS - SETTERS */
	void SetDepth(int Depth) { mDepth = Depth; }
	void GoUp() { mGoingDown = false; }
	void GoDown() { mGoingDown = true; }
	void SetBack(bool IsBack) { mIsBack = IsBack; }
private:
	/* PRIVATE VARIABLES */
	int mScore = 0;
	int mDepth = 0;
	bool mGoingDown = true;
	bool mIsBack = false;
	char mPlayerChar;
	std::vector<std::shared_ptr<Treasure>> mTreasureList;
};
