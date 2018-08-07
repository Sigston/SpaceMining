#pragma once
#include "Drawer.h"
#include "File.h"
#include "Treasure.h"
#include "Player.h"
#include <string>
#include <list>
#include <vector>

class Game
{
public:
	Game();
	~Game();

	void Play();
	void Initialize();
	void Introduction();
	bool Menu();
private:
	Drawer * Draw;
	File * Ship;

	std::vector<std::shared_ptr<Player>> PlayerList;
	std::vector<std::shared_ptr<Treasure>> TreasureList;
	std::vector<std::shared_ptr<Player>>::iterator ActivePlayer;

	struct
	{
		unsigned int ScreenWidth = 70;
		unsigned int Padding = 3;
		char ScreenChar = '|';
		std::string UserPrompt;
	} Settings;

	void PrintRules();
	void SetSettings();
	int RollDice();
	void Refresh();
	void NewTreasures();
	void NewPlayers(int HowMany);
	void DrawMine();

	// Utility functions:
	void ValidInputChar(char &OutputChar, std::string ErrorMessage);
	void ValidInputInt(int &OutputInt, std::string ErrorMessage);
	void ValidInputUInt(unsigned int &OutputInt, std::string ErrorMessage);
	void ValidInputStr(std::string &OutputStr, std::string ErrorMessage);
};
