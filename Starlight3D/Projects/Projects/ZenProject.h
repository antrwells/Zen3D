#pragma once
#include <vector>
#include <string>
class ZenProject
{
public:

	ZenProject();
	void SetName(std::string name);
	void SetAuthor(std::string author);
	void SetPath(std::string path);
	void SetInfo(std::string info);
	std::string GetName()
	{
		return mName;
	}
	std::string GetPath()
	{
		return mPath;
	}
	std::string GetAuthor() {
		return mAuthor;
	}

	std::string GetInfo() {
		return mInfo;
	}


private:

	std::string mName = "";
	std::string mPath = "";
	std::string mAuthor = "";
	std::string mInfo = "";

};

