#pragma once
#include <string>
#include <vector>

class File
{
public:
	/* CONSTRUCTOR */
	File(std::string FileName);

	/* PUBLIC FUNCTIONS */
	std::vector<std::string> GetFileTextAsVector();
	void OverwriteFile(std::string OverwriteString);
	bool UpdateFile();

	/* INLINE FUNCTIONS */
	std::string GetFileText() { return(mFileText); }
private:
	/* PRIVATE VARIABLES */
	const std::string mFileName;
	std::string mFileText;
};
