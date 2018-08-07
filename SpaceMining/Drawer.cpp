#include "Drawer.h"
#include <iostream>
#include <string>

Drawer::Drawer(unsigned int ScreenSpace, unsigned int Padding, char ScreenCharacter, std::string UserPrompt) :
	mScreenSpace(ScreenSpace), mPadding(Padding), mDrawSpace(ScreenSpace - 2 - Padding * 2),
	mScreenCharacter(ScreenCharacter), mUserPrompt(UserPrompt)
{ }

void Drawer::AddPadding(std::string &Result)
{
	for (int i = 0; i != mPadding; ++i)
	{
		Result.append(" ");
	}
}

void Drawer::DrawLine(std::string &Line)
{
	std::cout << MakeLine(Line);
}

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

void Drawer::Seperator()
{
	std::string Output(mScreenSpace, '=');
	std::cout << Output << "\n";
}

void Drawer::BlankLine()
{
	std::string Output;
	Output.append(1, mScreenCharacter);
	Output.append(mScreenSpace - 2, ' ');
	Output.append(1, mScreenCharacter);
	Output.append("\n");
	std::cout << Output;
}

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

void Drawer::UserPrompt()
{
	std::cout << mUserPrompt;
}
