#pragma once
#include "ZScriptNode.h"

class ZClassNode;

class ZMainNode : ZScriptNode
{
public:


	void AddClass(ZClassNode* node);
	void AddStaticClass(ZClassNode* node);
	std::vector<ZClassNode*> GetClasses();
	std::vector<ZClassNode*> GetStaticClasses();

private:

	std::vector<ZClassNode*> mClasses;
	std::vector<ZClassNode*> mStaticClasses;

};

