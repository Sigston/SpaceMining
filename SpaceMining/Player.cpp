#include "Player.h"

/* CONSTRUCTOR */

// Sets the char representing the player.
Player::Player(char PlayerChar) :
	mPlayerChar(PlayerChar)
{
}

/* PUBLIC FUNCTIONS - MOVEMENT */

// Returns the potential depth of any move, without making any changes. Call Move() to change mDepth. Accounts for
// movement up and down.
int Player::MoveResult(int Movement)
{
	Movement = (mGoingDown) ? Movement : Movement * -1;
	return(mDepth + Movement);
}

// Changes the depth of the player based on the given movement.
void Player::Move(int Movement) 
{ 
	mDepth = MoveResult(Movement);
}

/* PUBLIC FUNCTIONS - TREASURE */

// Adds a pointer to a treasure to the player's list of treasures.
void Player::AddTreasure(std::shared_ptr<Treasure> Treasure)
{
	mTreasureList.push_back(Treasure);
}

// Removes all treasures from the player.
void Player::DropTreasures()
{
	mTreasureList.clear();
}

// Returns true if the player has any treasures, false if they do not.
bool Player::HasTreasure()
{
	return((mTreasureList.empty()) ? false : true);
}

// Returns a string listing the player's treasures and their value.
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

/* PUBLIC FUNCTIONS - SCORE */

// Updates the player's score in accordance with the currently held treasures.
void Player::UpdateScore()
{
	for (auto it = mTreasureList.begin(); it < mTreasureList.end(); ++it)
	{
		mScore += (*it)->GetValue();
	}
}
