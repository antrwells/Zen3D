#pragma once
#include "ZScriptNode.h"

class ZClassNode;

class ZMainNode : ZScriptNode
{
public:


	void AddClass(ZClassNode* node);
	std::vector<ZClassNode*> GetClasses();

private:

	std::vector<ZClassNode*> mClasses;

};

