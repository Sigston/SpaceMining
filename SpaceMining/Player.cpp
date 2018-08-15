#include "Player.h"

Player::Player(char PlayerChar) :
	mPlayerChar(PlayerChar)
{
}

// Changes the depth of the player.
void Player::Move(int Movement) 
{ 
	mDepth = MoveResult(Movement);
}

// Returns the potential depth of any move, without making any changes.
int Player::MoveResult(int Movement)
{
	Movement = (mGoingDown) ? Movement : Movement * -1;
	return(mDepth + Movement);
}

// Adds a pointer to a treasure to the player's list of treasures.
void Player::PickUpTreasure(std::shared_ptr<Treasure> Treasure)
{
	mTreasureList.push_back(Treasure);
}

bool Player::HasTreasure()
{
	if (mTreasureList.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

std::string Player::PrintTreasures()
{
	std::string Output;
	if (mTreasureList.empty())
	{
		Output = " NOTHING!";
	}
	else
	{
		for (auto it = mTreasureList.begin(); it < mTreasureList.end(); ++it)
		{
			Output.append("\n    " + (*it)->GetType() + " of value " + std::to_string((*it)->GetValue()));
		}
		Output.append("\n");
	}
	return Output;
}

void Player::UpdateScore()
{
	for (auto it = mTreasureList.begin(); it < mTreasureList.end(); ++it)
	{
		mScore += (*it)->GetValue();
	}
}

void Player::DropTreasures()
{
	mTreasureList.clear();
}
