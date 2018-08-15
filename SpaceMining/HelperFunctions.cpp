#include "HelperFunctions.h"

/* 
All functions ensure input validation and are pretty similar; they all print the ErrorMessage until
the desired type is entered. ValidStr() is the only one to use std::getline(). The rest directly manipulate 
input streams. There is also a wait function which calls std::cin.ignore.
*/

void Input::ValidChar(char &OutputChar, std::string ErrorMessage)
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

void Input::ValidInt(int &OutputInt, std::string ErrorMessage)
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

void Input::ValidUInt(unsigned int &OutputInt, std::string ErrorMessage)
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

void Input::ValidStr(std::string &OutputStr, std::string ErrorMessage)
{
	std::getline(std::cin, OutputStr);
}

// Waits for user input. Calls std::cin.ignore().
void Input::Wait()
{
	std::cin.ignore();
}