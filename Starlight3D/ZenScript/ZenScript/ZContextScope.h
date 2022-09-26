#pragma once
#include <vector> 
#include <string>
#include <map>
#include "ZContextVar.h"

class ZClassNode;

class ZContextScope
{
public:

	ZContextScope(std::string name);
	void RegisterVar(ZContextVar* var);
	ZContextVar* FindVar(std::string name);
	void PushClass(ZClassNode* node);
	void PopClass(ZClassNode* node);

private:

	std::string mName;
	std::vector<ZContextVar*> mVars;
	std::map < std::string, ZContextVar*> mVarsMap;

};

