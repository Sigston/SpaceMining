#pragma once
#include <string>

class Drawer
{
private:
	const unsigned int mScreenSpace;
	const unsigned int mPadding;
	const unsigned int mDrawSpace;
	const char mScreenCharacter;
	const std::string mUserPrompt;

	void AddPadding(std::string &Result);
	void DrawLine(std::string &Line);
	std::string MakeLine(std::string &Line);

public:
	Drawer(unsigned int ScreenSpace, unsigned int Padding, char ScreenCharacter, std::string UserPrompt);
	void Seperator();
	void BlankLine();
	void CentralLine(std::string Text);
	void Text(std::string Text);
	void MultiLineText(std::string Text);
	void UserPrompt();
	std::string GetUserPrompt() { return mUserPrompt; }
};
