#pragma once
#include <vector>
#include <memory>
#include "Treasure.h"

class Player
{
public:
	Player(char PlayerChar);

	int GetDepth() { return(mDepth); }
	char GetChar() { return(mPlayerChar); }
	int GetScore() { return(mScore); }
	bool IsGoingDown() { return mGoingDown; }
	bool IsBack() { return mIsBack; }
	void SetDepth(int Depth) { mDepth = Depth; }
	void SetBack(bool IsBack) { mIsBack = IsBack; }
	void Move(int Movement);
	int MoveResult(int Movement);
	void GoUp() { mGoingDown = false; }
	void GoDown() { mGoingDown = true; }
	void PickUpTreasure(std::shared_ptr<Treasure> Treasure);
	bool HasTreasure();
	int GetTreasureNum() { return mTreasureList.size(); }
	std::string PrintTreasures();
	void DropTreasures();
	void UpdateScore();
private:
	int mScore = 0;
	int mDepth = 0;
	bool mGoingDown = true;
	bool mIsBack = false;
	char mPlayerChar;
	std::vector<std::shared_ptr<Treasure>> mTreasureList;
};
