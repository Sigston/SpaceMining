#pragma once
class Player
{
public:
	Player(char PlayerChar);

	int GetDepth() { return(mDepth); }
	char GetChar() { return(mPlayerChar); }
	void SetDepth(int Depth) { mDepth = Depth; }
	void Move(int Movement);
	void GoUp() { mGoingDown = false; }
private:
	int mScore = 0;
	int mDepth = 0;
	bool mGoingDown = true;
	char mPlayerChar;
};
