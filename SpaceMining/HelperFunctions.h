#pragma once
#include <string>
#include <iostream>

namespace Input
{
	void ValidChar(char &OutputChar, std::string ErrorMessage);
	void ValidInt(int &OutputInt, std::string ErrorMessage);
	void ValidUInt(unsigned int &OutputInt, std::string ErrorMessage);
	void ValidStr(std::string &OutputStr, std::string ErrorMessage);
	void Wait();
}