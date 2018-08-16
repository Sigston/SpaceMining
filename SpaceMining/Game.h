#pragma once
#include "Drawer.h"
#include "File.h"
#include "Treasure.h"
#include "TreasureGroup.h"
#include "Player.h"
#include "Random.h"
#include <string>
#include <vector>

class Game
{
public:
	Game();
	~Game();

	void Initialize();
	void Introduction();
	bool Menu();
	void Play();
private:
	/* PRIVATE VARIABLES */
	Drawer * Draw;
	File * Ship;
	File * TreasurePicsFile;
	Random * Randomizer;
	std::vector<std::string> TreasurePics;
	std::vector<std::shared_ptr<Player>> PlayerList;
	std::vector<std::shared_ptr<Treasure>> TreasureList;
	std::vector<std::shared_ptr<Player>>::iterator ActivePlayer;
	int RoundCounter = 0;
	int Oxygen = 25;

	struct
	{
		unsigned int ScreenWidth = 80;
		unsigned int Padding = 5;
		char ScreenChar = '|';
		std::string UserPrompt;
	} Settings;

	/* PRIVATE METHODS */

	/* Game Logic */
	void Refresh();
		void NewTreasures();
		void NewPlayers(int HowMany);
	void MovePlayer();
	void EndRound();
	void EndGame();

	/* Menu Logic */
	void SetSettings();

	/* Drawing Functions */
	void PrintIntro();
	void PrintRules();
	void PrintScores(char WinChar, int TopScore);
	void PrintScoresDraw(std::vector<std::shared_ptr<Player>> DrawnPlayers);
	void DrawBoard();
	bool DrawTreasures(int Begin, int End, std::vector<std::string> &Lines);

	/* Useful Functions */
	bool PlayerOfDepth(int Depth);
};
