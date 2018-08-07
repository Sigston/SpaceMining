// Implement Player class, vector of players and iterator to player whose current go it is.
// Implement Treasure class (has string of type and an associated value), vector of treasures.

#include "Game.h"
#include <time.h>
#include <algorithm>
#include <iostream>

Game::Game()
{
}
Game::~Game()
{
	if (Draw != nullptr)
	{
		delete Draw;
		Draw = nullptr;
	}
	if (Ship != nullptr)
	{
		delete Ship;
		Ship = nullptr;
	}
}

void Game::Play()
{
	Refresh();
	bool IsPlaying = true;
	while (IsPlaying)
	{
		Draw->BlankLine();
		Draw->MultiLineText(Ship->GetFileText());
		Draw->BlankLine();
		DrawMine();

		Draw->Text(std::string(1, (*ActivePlayer)->GetChar()) + " it is your go. Do you want to go " 
			"(1) down into the moon, or (2) back to the ship?");
		int Response = 0;
		while (Response < 1 || Response > 2)
		{
			Draw->UserPrompt();
			ValidInputInt(Response, Draw->GetUserPrompt());
		}
		if (Response == 2)
		{
			(*ActivePlayer)->GoUp();
		}
		int Movement = RollDice();
		if ((*ActivePlayer)->GetDepth() + RollDice() > TreasureList.size())
		{
			(*ActivePlayer)->SetDepth(TreasureList.size());
		}
		else
		{
			(*ActivePlayer)->Move(RollDice());
		}
		++ActivePlayer;
		if (ActivePlayer == PlayerList.end())
		{
			ActivePlayer = PlayerList.begin();
		}
	}
}

// Seeds the random number generator, sets up the drawer, and opens ascii art files.
void Game::Initialize()
{
	// Seed random number generator.
	srand((unsigned int)time(NULL));
	// Set up the drawing object.
	Settings.UserPrompt = std::string(1, Settings.ScreenChar) + std::string(Settings.Padding, ' ') + ": ";
	Draw = new Drawer(Settings.ScreenWidth, Settings.Padding, Settings.ScreenChar, Settings.UserPrompt);
	// Open the ascii art files.
	Ship = new File("Ship.txt");
}

void Game::Introduction()
{
	Draw->Seperator();
	Draw->BlankLine();
	Draw->CentralLine("SPACE MINING");
	Draw->BlankLine();
	Draw->Seperator();
	Draw->BlankLine();
	Draw->CentralLine("The Mine Is Mine!");
	Draw->BlankLine();
	Draw->BlankLine();
	Draw->CentralLine("A Game By Alex Sigston");
	Draw->BlankLine();
	Draw->Seperator();
}

bool Game::Menu()
{
	int Decision = 0;
	bool PlayGame;

	while (true)
	{
		Draw->BlankLine();
		Draw->Text("Would you like to (1) play a game, (2) see the rules, (3) quit the game, "
			"(4) change the settings?");

		// Get a valid decision
		while (Decision < 1 || Decision > 4)
		{
			Draw->UserPrompt();
			ValidInputInt(Decision, Draw->GetUserPrompt());
		}

		// Do the right thing
		switch (Decision)
		{
		case 4:
			SetSettings();
			break;
		case 3:
			PlayGame = false;
			break;
		case 2:
			PrintRules();
			break;
		case 1:
			PlayGame = true;
			break;
		}
		if (Decision == 1 || Decision == 3)
			break;
		Decision = 0;
	}
	return(PlayGame);
}

/* Private Methods */

void Game::PrintRules()
{
	Draw->Seperator();
	Draw->BlankLine();
	Draw->Text("Welcome to Space Mining! The year is 2056 and you and your friends are Deep Space Miners " 
		"working for Futuristic Mining Corp Ltd. You have been sent out to a distant moon of an even more " 
		"distant planet to mine the three minerals which your society depends on: Coppernium, Bronzium, Silverium and " 
		"the elusive Goldium. The miners, for reasons of economy and story convenience, have access to " 
		"the same tank of oxygen, and must compete with each other to gather more and more valuable mineral " 
		"deposits from deeper inside the lifeless moon.");
	Draw->BlankLine();
	Draw->Text("On each players go, they must decide to head deeper into the moon or back to the ship. " 
		"Once a miner has chosen to return, the ship begins to wind in their tether and they cannot decide " 
		"to head back into the fray. Each step on the way down into the moon holds one cache of minerals; "
		"but you only have time to tell which mineral they are, not its purity. Coppernium is found floating in the " 
		"atmosphere of the moon; Bronzium on the moon's surface; Silverium in the moon's crust; and Goldium in the moon's core. Once a miner has " 
		"picked up a cache of minerals, the oxygen begins to drain on their turn in proportion to the number " 
		"of caches they possess. These minerals are also heavy - the more you pick up, the harder it is to " 
		"reach the ship. When the oxygen is gone, all miners outside of the ship pass out, drop their " 
		"mineral deposits, which are drawn to the core of the moon by its gravity, and are dragged back " 
		"into the ship. Whoever can gather the greatest value of minerals, in purity and in type, is " 
		"awarded a bonus by Futuristic Mining Corp Ltd.");
	Draw->BlankLine();
	Draw->Text("Have Fun!");
	Draw->BlankLine();
	Draw->Seperator();
}

void Game::SetSettings()
{
	bool IsFinished = false;
	while (!IsFinished)
	{
		Draw->BlankLine();
		Draw->Seperator();
		Draw->BlankLine();
		Draw->Text("Change the (1) Screen Width, (2) Screen Padding, (3) Screen Character, "
			"(4) save changes and exit, or (5) restore defaults.");
		int Choice = 0;
		while (Choice < 1 || Choice > 5)
		{
			Draw->UserPrompt();
			ValidInputInt(Choice, Draw->GetUserPrompt());
		}
		switch (Choice)
		{
			case 5:
			{
				Settings.Padding = 3;
				Settings.ScreenChar = '|';
				Settings.ScreenWidth = 70;
				Settings.UserPrompt = std::string(1, Settings.ScreenChar) + 
					std::string(Settings.Padding, ' ') + ": ";
				break;
			}
			case 4:
			{
				IsFinished = true;
				break;
			}
			case 3:
			{
				Draw->Text("Enter a character.");
				char ScreenCharacter = '|';
				Draw->UserPrompt();
				ValidInputChar(ScreenCharacter, Draw->GetUserPrompt());
				Settings.ScreenChar = ScreenCharacter;
				break;
			}
			case 2:
			{
				Draw->Text("Enter a number between 1 and 10.");
				int Padding = 0;
				while (Padding < 1 || Padding > 10)
				{
					Draw->UserPrompt();
					ValidInputInt(Padding, Draw->GetUserPrompt());
				}
				Settings.Padding = Padding;
				break;
			}
			case 1:
			{
				Draw->Text("Enter a number between 50 and 100.");
				int ScreenWidth = 0;
				while (ScreenWidth < 50 || ScreenWidth > 100)
				{
					Draw->UserPrompt();
					ValidInputInt(ScreenWidth, Draw->GetUserPrompt());
				}
				Settings.ScreenWidth = ScreenWidth;
				break;
			}
		}
	}
	delete Draw;
	Settings.UserPrompt = std::string(1, Settings.ScreenChar) +
		std::string(Settings.Padding, ' ') + ": ";
	Draw = new Drawer(Settings.ScreenWidth, Settings.Padding, Settings.ScreenChar, Settings.UserPrompt);
	Draw->BlankLine();
	Draw->Seperator();
}

int Game::RollDice()
{
	int Dice1, Dice2;
	Dice1 =	rand() % 3 + 1;
	Dice2 = rand() % 3 + 1;
	return(Dice1 + Dice2);
}

void Game::Refresh()
{
	int NumberOfPlayers = 0;
	Draw->Text("How many players are there? Choose between 2 and 6.");
	while (NumberOfPlayers < 2 || NumberOfPlayers > 6)
	{
		Draw->UserPrompt();
		ValidInputInt(NumberOfPlayers, Draw->GetUserPrompt());
	}
	NewTreasures();
	NewPlayers(NumberOfPlayers);
}

void Game::NewTreasures()
{
	TreasureList.clear();
	int index = 0;
	for (int i = 0; i < 4; ++i)
	{
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Coppernium", i)));
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Coppernium", i)));
	}
	std::random_shuffle(TreasureList.begin() + index, TreasureList.end());
	for (int i = 4; i < 8; ++i)
	{
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Bronzium", i)));
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Bronzium", i)));
		index += 2;
	}
	std::random_shuffle(TreasureList.begin() + index, TreasureList.end());
	for (int i = 8; i < 12; ++i)
	{
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Silverium", i)));
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Silverium", i)));
		index += 2;
	}
	std::random_shuffle(TreasureList.begin() + index, TreasureList.end());
	for (int i = 12; i < 16; ++i)
	{
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Goldium", i)));
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Goldium", i)));
		index += 2;
	}
	std::random_shuffle(TreasureList.begin() + index, TreasureList.end());
}

void Game::NewPlayers(int HowMany)
{
	PlayerList.clear();
	for (int i = 0; i < HowMany; ++i)
	{
		PlayerList.push_back(std::shared_ptr<Player>(new Player(49 + i)));
	}
	ActivePlayer = PlayerList.begin();
}

void Game::DrawMine()
{
	std::string Output = std::string(TreasureList.size(), ' ');
	for (auto it = PlayerList.begin(); it < PlayerList.end(); ++it)
	{
		if ((*it)->GetDepth() > 0)
		{
			Output[(*it)->GetDepth()] = (*it)->GetChar();
		}
	}
	Output.append("\n");
	for (auto it = TreasureList.begin(); it < TreasureList.end(); ++it)
	{
		Output.append(std::string(1, (*it)->GetChar()));
	}
	Output.append("\n");
	Draw->MultiLineText(Output);
}

/* Utility Functions */

void Game::ValidInputChar(char &OutputChar, std::string ErrorMessage)
{
	std::cin >> OutputChar;
	while (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << ErrorMessage;
		std::cin >> OutputChar;
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void Game::ValidInputInt(int &OutputInt, std::string ErrorMessage)
{
	std::cin >> OutputInt;
	while (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << ErrorMessage;
		std::cin >> OutputInt;
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void Game::ValidInputUInt(unsigned int &OutputInt, std::string ErrorMessage)
{
	std::cin >> OutputInt;
	while (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << ErrorMessage;
		std::cin >> OutputInt;
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void Game::ValidInputStr(std::string &OutputStr, std::string ErrorMessage)
{
	std::getline(std::cin, OutputStr);
}