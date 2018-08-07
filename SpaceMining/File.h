#pragma once
#include <string>

class File
{
	const std::string mFileName;
	std::string mFileText;
public:
	File(std::string FileName);
	bool UpdateFile();
	std::string GetFileText();
	void OverwriteFile(std::string OverwriteString);
};
