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
	void RemoveVar(ZContextVar* Var);
	//ZContextVar* FindVar(std::string name);
	ZContextVar* FindVar(size_t hash);
	std::vector<ZContextVar*> GetVars();
	void SetVars(std::vector<ZContextVar*> vars);
	void PushClass(ZClassNode* node);
	void PopClass(ZClassNode* node);
	ZContextScope* Clone();


private:

	std::string mName;
	std::vector<ZContextVar*> mVars;
	//std::map < std::string, ZContextVar*> mVarsMap;


};

