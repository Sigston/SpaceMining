#include "Game.h"

int main()
{
	Game * game = new Game();
	game->Initialize();
	game->Introduction();
	while (game->Menu())
	{
		game->Play();
	}
	delete game;
	return(0);
}
