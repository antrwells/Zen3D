#pragma once
#include "ZProgramNode.h"
#include <string>
class ZContextScope;

class ZScriptContext
{
public:

	ZScriptContext();
	void AddNode(ZMainNode* node);
	ZClassNode* CreateInstance(std::string name,std::string instance_name);


private:

	std::vector<ZClassNode*> mClasses;


};

