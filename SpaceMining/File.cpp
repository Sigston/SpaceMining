#include "File.h"
#include <string>
#include <fstream>

/*
The File class reads out a file on creation into a private string. Then all operations on the object are on
the string associated with the file. UpdateFile() affects the physical file. The Filename cannot be changed.
*/

/* CONSTRUCTOR */

// Opens a file with the given file name and dumps its contents into mFileText. If the file cannot be opened,
// the mFileText contains an error message.
File::File(std::string FileName) : mFileName(FileName)
{
	std::ifstream File(mFileName);
	if (File.is_open())
	{
		char buffer[4096];
		while (File.read(buffer, sizeof(buffer)))
			mFileText.append(buffer, sizeof(buffer));
		mFileText.append(buffer, File.gcount());
		File.close();
	}
	else
	{
		mFileText = "ERROR OPENING FILE.";
	}
}

/* PUBLIC FUNCTIONS */

// Returns each line of text in the file as a string in a vector, new line characters removed. Will return an empty
// vector is the file has no new line characters (even if it has text).
std::vector<std::string> File::GetFileTextAsVector()
{
	std::vector<std::string> Output;
	std::string Line;
	auto Begin = mFileText.begin();
	for (auto it = mFileText.begin(); it < mFileText.end(); ++it)
	{
		if (*it == '\n')
		{
			Line = std::string(Begin, it + 1);
			Output.push_back(Line);
			Line.clear();
			Begin = it + 1;
		}
	}
	return(Output);
}

// Replaces the stored mFileText with the given string. Call UpdateFile() to affect the actual file.
void File::OverwriteFile(std::string OverwriteString)
{
	mFileText = OverwriteString;
}

// Copies across the mFileText to the physical file. Returns false if the file cannot be opened. 
bool File::UpdateFile()
{
	std::ofstream File(mFileName);
	if (File.is_open())
	{
		File << mFileText;
		return true;
	}
	else
	{
		return false;
	}
}
