#pragma once
#include "ZScriptNode.h"

class ZClassNode;

class ZProgramNode : ZScriptNode
{
public:


	void AddClass(ZClassNode* node);

private:

	std::vector<ZClassNode*> mClasses;

};

