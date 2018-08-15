#include "Random.h"
#include <time.h>
#include <cstdlib>

/* CONSTRUCTOR */

Random::Random()
{
}

/* PUBLIC FUNCTIONS */

// Seeds the standard C random number generator.
void Random::Init()
{
	srand((unsigned int)time(NULL));
}

// Returns the equivalent of the sum of two six sided dice, each numbered 1, 1, 2, 2, 3, 3.
int Random::RollDice()
{
	int Dice1, Dice2;
	Dice1 = rand() % 3 + 1;
	Dice2 = rand() % 3 + 1;
	return(Dice1 + Dice2);
}
