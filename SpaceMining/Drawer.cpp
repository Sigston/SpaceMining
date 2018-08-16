#include "Drawer.h"
#include <iostream>
#include <string>

/* CONSTRUCTOR */

// Assigns all the variables in the Draw object. These are all const - to change settings, delete and create a new Drawer object.
Drawer::Drawer(unsigned int ScreenSpace, unsigned int Padding, char ScreenCharacter, std::string UserPrompt) :
	mScreenSpace(ScreenSpace), mPadding(Padding), mDrawSpace(ScreenSpace - 2 - Padding * 2),
	mScreenCharacter(ScreenCharacter), mUserPrompt(UserPrompt)
{ }

/* PUBLIC DRAWING FUNCTIONS */

// Prints a string of text according to the given settings. For a string with '\n' characters, use MultiLineText().
void Drawer::Text(std::string Text)
{
	while (Text.length() > mDrawSpace)
	{
		int Index = mDrawSpace;
		while (Text[Index] != ' ')
		{
			if (Index == 0)
				break;
			--Index;
		}
		if (Index == 0)
		{
			Index = mDrawSpace;
		}
		std::string Output = Text.substr(0, Index);
		DrawLine(Output);
		std::cout << "\n";
		if (Text[Index] == ' ')
			Text.erase(0, Index + 1);
		else
			Text.erase(0, Index);
	}
	DrawLine(Text);
	std::cout << "\n";
}

// Prints a string of text according to the given settings, with user supplied new lines. For a string with no
// '\n' characters, use Text().
void Drawer::MultiLineText(std::string Text)
{
	std::string Output;
	for (auto it = Text.begin(); it != Text.end(); ++it)
	{
		if (*it == '\n')
		{
			Drawer::Text(Output);
			Output.clear();
		}
		else
			Output.append(1, *it);
	}
}

// Prints one centralized line of text according to the given settings. Prints an error message if this line
// is too long.
void Drawer::CentralLine(std::string Text)
{
	if (Text.length() < mDrawSpace)
	{
		std::string Output;
		Output.append(1, mScreenCharacter);
		Output.append(mPadding, ' ');

		int WhiteSpace = mDrawSpace - Text.length();
		Output.append(WhiteSpace / 2, ' ');
		Output.append(Text);
		if (WhiteSpace % 2 == 0)
			Output.append(WhiteSpace / 2, ' ');
		else
			Output.append(WhiteSpace / 2 + 1, ' ');
		Output.append(mPadding, ' ');
		Output.append(1, mScreenCharacter);
		Output.append(1, '\n');
		std::cout << Output;
	}
	else
	{
		std::cout << "ERROR: TEXT TOO LONG.\n";
	}
}

// Prints a line of '=' as a seperator.
void Drawer::Seperator()
{
	std::string Output(mScreenSpace, '=');
	std::cout << Output << "\n";
}

// Prints a line of spaces according to the given settings.
void Drawer::BlankLine(int HowMany)
{
	for (int i = 0; i < HowMany; ++i)
	{
		std::string Output;
		Output.append(1, mScreenCharacter);
		Output.append(mScreenSpace - 2, ' ');
		Output.append(1, mScreenCharacter);
		Output.append("\n");
		std::cout << Output;
	}
}

// Prints a line of spaces, the seperator, and a line of spaces.
void Drawer::SpaceSeperator()
{
	BlankLine();
	Seperator();
	BlankLine();
}

// Prints the user prompt.
void Drawer::UserPrompt()
{
	std::cout << mUserPrompt;
}

/* PRIVATE FUNCTIONS */

// Prints a single string according to the given settings. Calls MakeLine() to format the string.
void Drawer::DrawLine(std::string &Line)
{
	std::cout << MakeLine(Line);
}

// Formats a single string according to the given settings. Call DrawLine() to draw it.
std::string Drawer::MakeLine(std::string &Line)
{
	std::string Result;

	Result.append(1, mScreenCharacter);
	AddPadding(Result);
	Result.append(Line);
	int Space = mDrawSpace - Line.length();
	Result.append(Space, ' ');
	AddPadding(Result);
	Result.append(1, mScreenCharacter);

	return(Result);
}

// Adds padding to the end of a string according to the given settings.
void Drawer::AddPadding(std::string &Result)
{
	for (int i = 0; i != mPadding; ++i)
	{
		Result.append(" ");
	}
}
