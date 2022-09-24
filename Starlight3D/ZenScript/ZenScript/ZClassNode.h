#pragma once
#include "ZScriptNode.h"
#include <string>
#include <vector>
#include <map>

class ZMethodNode;

class ZClassNode : public ZScriptNode
{
public:

	void SetName(std::string name);
	void AddMethod(ZMethodNode* node);

private:
	
	std::string mClassName;
	std::string mInherits;
	
	//funcs

	//meths
	std::vector<ZMethodNode*> mMethods;

};

