#pragma once
#include <string>

class Drawer
{
public:
	/* CONSTRUCTOR */
	Drawer(unsigned int ScreenSpace, unsigned int Padding, char ScreenCharacter, std::string UserPrompt);

	/* PUBLIC DRAWING FUNCTIONS */
	void Text(std::string Text);
	void MultiLineText(std::string Text);
	void CentralLine(std::string Text);
	void Seperator();
	void BlankLine(int HowMany = 1);
	void SpaceSeperator();
	void UserPrompt();

	/* INLINE FUNCTIONS */
	std::string GetUserPrompt() { return(mUserPrompt); }
private:
	/* SCREEN VARIABLES */
	const unsigned int mScreenSpace;
	const unsigned int mPadding;
	const unsigned int mDrawSpace;
	const char mScreenCharacter;
	const std::string mUserPrompt;

	/* PRIVATE FUNCTIONS */
	void DrawLine(std::string &Line);
	std::string MakeLine(std::string &Line);
	void AddPadding(std::string &Result);
};
