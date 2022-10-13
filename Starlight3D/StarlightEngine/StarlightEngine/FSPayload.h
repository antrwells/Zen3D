#pragma once
#include<string>
#include<vector>
#include <map>
#include "FSFile.h"
#include <thread>

class FSPayload
{
public:
	FSPayload(std::string name);
	void AddFile(std::string name,std::string path, ResourceType type);
	void Load();
	void LoadThreaded();
	void* GetResource(std::string name);
	FSResource* GetResourceObj(std::string name);
	bool Loaded();
	std::vector<FSResource*> GetResources();
	float LoadedPercent();

private:
	std::string mName;
	std::vector<FSResource*> mResources;
	std::map<std::string, FSResource*> mLoaded;
	std::thread load_thread;
};

