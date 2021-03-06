#include "Game.h"
#include "HelperFunctions.h"
#include "Random.h"
#include <algorithm>

/* CONSTRUCTOR AND DESCTRUCTOR*/

// Does nothing - initialization is done in the Initialize() function.
Game::Game()
{
}

// Deals with the only pointers to memory not managed by smart pointers.
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
	if (TreasurePicsFile != nullptr)
	{
		delete TreasurePicsFile;
		TreasurePicsFile = nullptr;
	}
	if (Randomizer != nullptr)
	{
		delete Randomizer;
		Randomizer = nullptr;
	}
}

/* PUBLIC METHODS */

// Sets up the drawer and randomizer, and opens art files to store them appropriately in memory.
// Should be called before Run().
void Game::Initialize()
{
	Randomizer->Init();
	Settings.UserPrompt = std::string(1, Settings.ScreenChar) + std::string(Settings.Padding, ' ') + ": ";
	Draw = new Drawer(Settings.ScreenWidth, Settings.Padding, Settings.ScreenChar, Settings.UserPrompt);
	Ship = new File("Ship.txt");
	TreasurePicsFile = new File("Treasures.txt");
	std::vector<std::string> Temp = TreasurePicsFile->GetFileTextAsVector();
	std::string TempString;
	int Counter = 0;
	for (auto it = Temp.begin(); it < Temp.end(); ++it)
	{
		TempString.append(*it);
		++Counter;
		if (Counter == 3)
		{
			TreasurePics.push_back(TempString);
			TempString.clear();
			Counter = 0;
		}
	}
}

// Prints the introduction to the game.
void Game::Introduction()
{
	Draw->Seperator();
	Draw->BlankLine();
	Draw->CentralLine("SPACE MINING");
	Draw->SpaceSeperator();
	Draw->CentralLine("The Mine Is Mine!");
	Draw->BlankLine(2);
	Draw->CentralLine("A Game By Alex Sigston");
	Draw->SpaceSeperator();
}

// Allows the player to play the game, see the rules, quit, or change settings.
// Returns true if the player wants to play, and false if they wish to quit the game.
bool Game::Menu()
{
	int Decision = 0;
	bool PlayGame;
	while (true)
	{
		Draw->BlankLine();
		Draw->Text("Would you like to (1) play a game, (2) see the rules, (3) quit the game, or "
			"(4) change the settings?");

		while (Decision < 1 || Decision > 4)
		{
			Draw->UserPrompt();
			Input::ValidInt(Decision, Draw->GetUserPrompt());
		}
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

// Runs the main game logic. When Play() returns, the game is over.
void Game::Play()
{
	Refresh();
	PrintIntro();
	Input::Wait();
	bool IsPlaying = true;
	while (IsPlaying)
	{
		Oxygen -= (*ActivePlayer)->GetTreasureNum();
		PrintBoard();
		// If the player is going down, check if the player wants to go up or down. Move them.
		if ((*ActivePlayer)->IsGoingDown())
		{
			Draw->Text("Player " + std::string(1, (*ActivePlayer)->GetChar()) + " it is your go. Do you want to go "
				"(1) down into the moon, or (2) back to the ship?");
			int Response = 0;
			while (Response < 1 || Response > 2)
			{
				Draw->UserPrompt();
				Input::ValidInt(Response, Draw->GetUserPrompt());
			}
			if (Response == 2)
			{
				(*ActivePlayer)->GoUp();
			}
		}
		MovePlayer();
		// Check if they want to pick up minerals where they have landed.
		if (((*ActivePlayer)->GetDepth() != 0) && (!TreasureList[(*ActivePlayer)->GetDepth() - 1]->IsTaken()))
		{
			Draw->Text("There is " + TreasureList[(*ActivePlayer)->GetDepth() - 1]->GetType() + " here. Do you want to mine it? (1) Yes or (2) no.");
			int Response = 0;
			while (Response < 1 || Response > 2)
			{
				Draw->UserPrompt();
				Input::ValidInt(Response, Draw->GetUserPrompt());
			}
			if (Response == 1)
			{
				(*ActivePlayer)->AddTreasure(TreasureList[(*ActivePlayer)->GetDepth() - 1]);
				TreasureList[(*ActivePlayer)->GetDepth() - 1]->Take();
			}
		}
		// Check if all players are back home, if not, check if all the oxygen is gone, if not, change the players and go again.
		int Counter = 0;
		for (auto it = PlayerList.begin(); it < PlayerList.end(); ++it)
		{
			if ((*it)->GetDepth() == 0 && (*it)->IsBack())
			{
				++Counter;
			}
		}
		if (Counter == PlayerList.size() || Oxygen <= 0)
		{
			EndRound();
			if (RoundCounter >= 3)
			{
				IsPlaying = false;
			}
			Input::Wait();
		}
		else
		{
			do
			{
				++ActivePlayer;
				if (ActivePlayer == PlayerList.end())
				{
					ActivePlayer = PlayerList.begin();
				}
			} while ((*ActivePlayer)->IsBack());
		}
	}
	EndGame();
}

/* PRIVATE METHODS */

/* Game Logic */

// Called at the beginning of a game. Sets the number of players. 
// Calls NewTreasures() and NewPlayers().
void Game::Refresh()
{
	int NumberOfPlayers = 0;
	Draw->Text("How many players are there? Choose between 2 and 6.");
	while (NumberOfPlayers < 2 || NumberOfPlayers > 6)
	{
		Draw->UserPrompt();
		Input::ValidInt(NumberOfPlayers, Draw->GetUserPrompt());
	}
	NewTreasures();
	NewPlayers(NumberOfPlayers);
}

// Populates the TreasureList with the default values.
void Game::NewTreasures()
{
	TreasureList.clear();
	// The treasure list is four groups of eight treasures, random within each group.
	int index = 0;
	for (int i = 0; i < 4; ++i)
	{
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Coppernium", i, TreasurePics[0])));
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Coppernium", i, TreasurePics[0])));
	}
	std::random_shuffle(TreasureList.begin() + index, TreasureList.end());
	for (int i = 4; i < 8; ++i)
	{
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Bronzium", i, TreasurePics[1])));
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Bronzium", i, TreasurePics[1])));
		index += 2;
	}
	std::random_shuffle(TreasureList.begin() + index, TreasureList.end());
	for (int i = 8; i < 12; ++i)
	{
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Silverium", i, TreasurePics[2])));
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Silverium", i, TreasurePics[2])));
		index += 2;
	}
	std::random_shuffle(TreasureList.begin() + index, TreasureList.end());
	for (int i = 12; i < 16; ++i)
	{
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Goldium", i, TreasurePics[3])));
		TreasureList.push_back(std::shared_ptr<Treasure>(new Treasure("Goldium", i, TreasurePics[3])));
		index += 2;
	}
	std::random_shuffle(TreasureList.begin() + index, TreasureList.end());
}

// Populates the PlayerList with the required number of players.
void Game::NewPlayers(int HowMany)
{
	PlayerList.clear();
	for (int i = 0; i < HowMany; ++i)
	{
		PlayerList.push_back(std::shared_ptr<Player>(new Player('1' + i)));
	}
	ActivePlayer = PlayerList.begin();
}

// Manages the logic of moving the player. Also alerts the player how they have moved.
void Game::MovePlayer()
{
	// Constrain movement if going up by the weight of materials.
	int Movement = Randomizer->RollDice();
	if (!(*ActivePlayer)->IsGoingDown())
	{
		Movement -= (*ActivePlayer)->GetTreasureNum();
	}
	// We go through each piece of movement and check if anything needs to be done.
	for (int i = 0; i < Movement; ++i)
	{
		// First we check if moving once puts the player over the end of the Treasure List
		// or back to the ship.
		if ((*ActivePlayer)->MoveResult(1) > TreasureList.size())
		{
			break;
		}
		else if ((*ActivePlayer)->MoveResult(1) == 0)
		{
			(*ActivePlayer)->SetDepth(0);
			break;
		}
		// Now we deal with a potential clash with another player.
		int CurrentPosition = (*ActivePlayer)->GetDepth();
		int PotentialPosition = (*ActivePlayer)->MoveResult(1);
		if (PlayerOfDepth(PotentialPosition))
		{
			while (PlayerOfDepth(PotentialPosition))
			{
				int Increment = ((*ActivePlayer)->IsGoingDown()) ? 1 : -1;
				PotentialPosition += Increment;
			}
			if ((*ActivePlayer)->MoveResult(PotentialPosition - CurrentPosition) >
				TreasureList.size())
			{
				(*ActivePlayer)->SetDepth(CurrentPosition);
				break;
			}
			else if ((*ActivePlayer)->MoveResult(abs(PotentialPosition - CurrentPosition)) < 0)
			{
				(*ActivePlayer)->SetDepth(0);
				break;
			}
			else
			{
				(*ActivePlayer)->Move(abs(PotentialPosition - CurrentPosition));
			}
		}
		else
		{
			(*ActivePlayer)->Move(1);
		}
	}
	// Alert the player of movements.
	if ((*ActivePlayer)->GetDepth() == 0)
	{
		(*ActivePlayer)->SetBack(true);
		Draw->Text("Player " + std::string(1, (*ActivePlayer)->GetChar()) + ", you have made it back to the ship!");
		Input::Wait();
	}
	else
	{
		std::string Notice;
		std::string Cavern = (Movement > 1) ? "caverns" : "cavern";
		if (Movement <= 0)
		{
			Notice = "The mineral deposits you have mined weigh you down. You make no progress this turn.";
		}
		else if ((*ActivePlayer)->IsGoingDown())
		{
			Notice = "You descend " + std::to_string(Movement) + " " + Cavern + " into the moon.";
		}
		else
		{
			Notice = "Player " + std::string(1, (*ActivePlayer)->GetChar()) + " floats up " + std::to_string(Movement) +
				" " + Cavern + " out of the moon.";
		}
		Draw->Text(Notice);
		Input::Wait();
	}
}

// Called when all players have returned to the ship, or the oxygen has ran out.
void Game::EndRound()
{
	PrintBoard();
	std::string Scores;
	auto FirstPlayer = PlayerList.begin();
	int DeepestDepth = 0;
	// Iterate through each player and deal with their end round state and scores; print this data.
	for (auto it = PlayerList.begin(); it < PlayerList.end(); ++it)
	{
		if (!(*it)->HasTreasure())
		{
			Draw->Text("Player " + std::string(1, (*it)->GetChar()) + " picked up no mineral deposits!");
		}
		else if ((*it)->GetDepth() != 0)
		{
			Draw->Text("Player " + std::string(1, (*it)->GetChar()) + " ran out of oxygen and was dragged back to the ship!");
			if ((*it)->HasTreasure())
			{
				TreasureList.push_back(std::shared_ptr<Treasure> (new TreasureGroup(TreasurePics[5], (*it)->GetTreasures())));
			}
			if ((*it)->GetDepth() > DeepestDepth)
			{
				DeepestDepth = (*it)->GetDepth();
				FirstPlayer = it;
			}
		}
		else
		{
			Draw->MultiLineText("Player " + std::string(1, (*it)->GetChar()) + " picked up:" + (*it)->PrintTreasures());
			(*it)->UpdateScore();
		}
		Scores.append("Player " + std::string(1, (*it)->GetChar()) + " now has " + std::to_string((*it)->GetScore()) + " points.\n");
		(*it)->DropTreasures();
		(*it)->SetDepth(0);
		(*it)->SetBack(false);
		(*it)->GoDown();
	}
	Draw->MultiLineText(Scores);
	// Deal with taken treasures.
	for (auto it = TreasureList.begin(); it < TreasureList.end(); )
	{
		if ((*it)->IsTaken())
		{
			it = TreasureList.erase(it);
		}
		else
		{
			++it;
		}
	}
	// Refresh statuses.
	++RoundCounter;
	Oxygen = 25;
	ActivePlayer = FirstPlayer;
}

// Calculates scores and prints them. Calls PrintScores().
void Game::EndGame()
{
	std::vector<std::shared_ptr<Player>> HighScorePlayers;
	int TopScore = (*PlayerList.begin())->GetScore();
	for (auto it = PlayerList.begin() + 1; it < PlayerList.end(); ++it)
	{
		if ((*it)->GetScore() > TopScore)
		{
			TopScore = (*it)->GetScore();
		}
	}
	for (auto it = PlayerList.begin(); it < PlayerList.end(); ++it)
	{
		if ((*it)->GetScore() == TopScore)
		{
			HighScorePlayers.push_back((*it));
		}
	}
	if (HighScorePlayers.size() == 1)
	{
		PrintScores((*HighScorePlayers.begin())->GetChar(), (*HighScorePlayers.begin())->GetScore());
	}
	else
	{
		PrintScoresDraw(HighScorePlayers);
	}
}

/* Menu Logic */

// Allow the player to set the settings. Called by Menu().
void Game::SetSettings()
{
	bool IsFinished = false;
	while (!IsFinished)
	{
		Draw->SpaceSeperator();
		Draw->Text("Change the (1) Screen Width, (2) Screen Padding, (3) Screen Character, "
			"(4) save changes and exit, or (5) restore defaults.");
		int Choice = 0;
		while (Choice < 1 || Choice > 5)
		{
			Draw->UserPrompt();
			Input::ValidInt(Choice, Draw->GetUserPrompt());
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
			Input::ValidChar(ScreenCharacter, Draw->GetUserPrompt());
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
				Input::ValidInt(Padding, Draw->GetUserPrompt());
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
				Input::ValidInt(ScreenWidth, Draw->GetUserPrompt());
			}
			Settings.ScreenWidth = ScreenWidth;
			break;
		}
		}
	}
	// Refresh the Draw object pointed to with the new settings.
	delete Draw;
	Settings.UserPrompt = std::string(1, Settings.ScreenChar) +
		std::string(Settings.Padding, ' ') + ": ";
	Draw = new Drawer(Settings.ScreenWidth, Settings.Padding, Settings.ScreenChar, Settings.UserPrompt);
}

/* Drawing Functions */

void Game::PrintIntro()
{
	Draw->SpaceSeperator();
	Draw->Text("Welcome to Space Mining! The year is 2056 and you and your friends are Deep Space Miners "
		"working for Futuristic Mining Corp Ltd. You have been sent out to a distant moon of an even more "
		"distant planet to mine the three minerals which your society depends on: Coppernium, Bronzium, Silverium and "
		"the elusive Goldium. The miners, for reasons of economy and story convenience, have access to "
		"the same tank of oxygen, and must compete with each other to gather more and more valuable mineral "
		"deposits from deeper inside the lifeless moon.");
}

// Prints the rules. Called by Menu().
void Game::PrintRules()
{
	PrintIntro();
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
	Draw->SpaceSeperator();
}

// Takes the player character and the topscore and prints the end of game score spiel.
void Game::PrintScores(char WinChar, int TopScore)
{
	Draw->SpaceSeperator();
	Draw->CentralLine("GAME OVER");
	Draw->BlankLine();
	Draw->Text("Player " + std::string(1, WinChar) + " has won with " + std::to_string(TopScore) + " points. "
		"You receive several hundred credits and a free trip home to see your family. The other players must "
		"spend the rest of their lives mining minerals in the most distant part of the solar system. They also "
		"have to listen to post-Peter Gabriel Genesis albums.");
	Draw->SpaceSeperator();
	Input::Wait();
}

// Takes a vector of players who have drawn and prints the end of game score spiel.
void Game::PrintScoresDraw(std::vector<std::shared_ptr<Player>> DrawnPlayers)
{
	std::string DrawNature;
	switch (DrawnPlayers.size())
	{
	case 2:
		DrawNature = "two";
		break;
	case 3:
		DrawNature = "three";
		break;
	case 4:
		DrawNature = "four";
		break;
	case 5:
		DrawNature = "five";
		break;
	case 6:
		DrawNature = "six";
		break;
	}
	std::string Players;
	for (auto it = DrawnPlayers.begin(); it < DrawnPlayers.end() - 1; ++it)
	{
		Players.append("Player " + std::string(1, (*it)->GetChar()) + ", ");
	}
	Players.append("and Player " + std::string(1, (*(DrawnPlayers.end() - 1))->GetChar()));

	Draw->SpaceSeperator();
	Draw->CentralLine("GAME OVER");
	Draw->BlankLine();
	Draw->Text("It is that most interesting of game endings: a draw. " + Players + " all scored " 
		+ std::to_string((*DrawnPlayers.begin())->GetScore()) + " points. This gives us a " + DrawNature + "-fold draw. " 
		"The company stipulates that in the event of a draw, no one receives a bonus and mining must continue. " 
		"This saddens all of you, but you thank the creator of the game for his efforts and resolve to play again in the " 
		"event that you might win next time. In the mean time, have a cup of tea and put your feet up.");
	Draw->SpaceSeperator();
	Input::Wait();
}

// Draws the ship, the minerals and the players.
void Game::PrintBoard()
{
	Draw->SpaceSeperator();
	// Put together the ship part of the board.
	std::string ShipString = Ship->GetFileText();
	// Put in oxygen data:
	size_t Offset = ShipString.find("OXYGEN: ") + std::string("OXYGEN: ").size();
	ShipString.insert(Offset, std::to_string(Oxygen));
	// Put in score data and safe players:
	std::string Scores;
	std::string SafePlayers;
	for (auto it = PlayerList.begin(); it < PlayerList.end(); ++it)
	{
		Scores.append(std::string(1, (*it)->GetChar()) + ": " + std::to_string((*it)->GetScore()) + "   ");
		if ((*it)->IsBack())
		{
			SafePlayers.append(std::string(1, (*it)->GetChar()) + " ");
		}
	}
	Offset = ShipString.find("SCORE: ") + std::string("SCORE: ").size();
	ShipString.insert(Offset, Scores);
	Offset = ShipString.find("SAFE: ") + std::string("SAFE: ").size();
	ShipString.insert(Offset, SafePlayers);
	// Print:
	Draw->MultiLineText(ShipString);
	Draw->BlankLine();

	// Now the mines and the players.
	std::string MineString; // This is eventually sent to the Draw object.
	std::vector<std::string> Lines = { "", "", "" }; // Each treasure object is three lines deep.
	int Begin = 0;
	int TreasuresPerLine = 8;
	std::string SymbolString;
	bool RemainingTreasures = true;
	while (RemainingTreasures)
	{
		if ((Begin + TreasuresPerLine) > TreasureList.size())
		{
			FormatTreasures(Begin, TreasureList.size(), Lines);
			RemainingTreasures = false;
		}
		else
		{
			FormatTreasures(Begin, Begin + TreasuresPerLine, Lines);
			Begin += TreasuresPerLine;
			if (Begin == TreasureList.size()) // In case the number of treasures is a multiple of TreasureList.size()
			{
				RemainingTreasures = false;
			}
		}
		MineString.append(Lines[0] + "\n");
		MineString.append(Lines[1] + "\n");
		MineString.append(Lines[2] + "\n\n");
		Lines = { "", "", "" };
	}
	// Print all of the above:
	Draw->MultiLineText(MineString);
	Draw->SpaceSeperator();
}

// Takes the begin and end indices of the TreasureList and builds the symbols onto a vector of strings.
// Returns false if the End integer is greater than the size of the TreasureList.
bool Game::FormatTreasures(int Begin, int End, std::vector<std::string> &Lines)
{
	if (End > TreasureList.size())
	{
		return(false);
	}
	std::string SymbolString;
	for (; Begin < End; ++Begin)
	{
		int Start = 0;
		int Counter = 0;
		SymbolString = (TreasureList[Begin]->IsTaken()) ? TreasurePics[4] : TreasureList[Begin]->GetSymbol();
		for (int i = 0; i < SymbolString.size(); ++i)
		{
			if (SymbolString[i] == '\n')
			{
				std::string Temp = SymbolString.substr(Start, i - Start);
				if (Counter == 1)
				{
					for (auto it = PlayerList.begin(); it < PlayerList.end(); ++it)
					{
						if ((*it)->GetDepth() == Begin + 1)
						{
							Temp[2] = (*it)->GetChar();
						}
					}
				}
				Lines[Counter].append(Temp);
				Start = i + 1;
				++Counter;
			}
		}
		Lines[0].append("   ");
		Lines[1].append("   ");
		Lines[2].append("   ");
	}
	return(true);
}

/* Useful Functions */

// Returns true if a player of the supplied depth exists. Does not account for duplicates.
bool Game::PlayerOfDepth(int Depth)
{
	for (auto it = PlayerList.begin(); it < PlayerList.end(); ++it)
	{
		if ((*it)->GetDepth() == Depth)
		{
			return true;
		}
	}
	return false;
}
