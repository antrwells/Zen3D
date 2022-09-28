#pragma once
#include "ZProgramNode.h"
#include <string>
//#include "DataTypes.h"

#include "ZContextVar.h"
class ZContextScope;

class ZScriptContext
{
public:

	ZScriptContext();
	void SetupSystem();
	void AddNode(ZMainNode* node);
	ZClassNode* CreateInstance(std::string name,std::string instance_name);

	static ZContextScope* CurrentScope;

private:

	std::vector<ZClassNode*> mClasses;


};

