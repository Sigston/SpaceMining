#include "File.h"
#include <string>
#include <fstream>

// Class object always has the same filename. Reads out the file on creation, then all
// operations on the "file" are on a string associated with the file. Only UpdateFile()
// affects the physical file.
File::File(std::string FileName) :
	mFileName(FileName)
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

// Copies across the mFileText to the physical file. Will return false if the file
// cannot be opened. Should be called before any file operations are performed.
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

std::string File::GetFileText()
{
	return mFileText;
}
void File::OverwriteFile(std::string OverwriteString)
{
	mFileText = OverwriteString;
}
