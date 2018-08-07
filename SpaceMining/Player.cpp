#include "Player.h"

Player::Player(char PlayerChar) :
	mPlayerChar(PlayerChar)
{
}

void Player::Move(int Movement) 
{ 
	Movement = (mGoingDown) ? Movement : Movement * -1;
	mDepth += Movement; 
}
