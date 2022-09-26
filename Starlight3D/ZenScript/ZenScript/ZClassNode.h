#pragma once
#include "ZScriptNode.h"
#include <string>
#include <vector>
#include <map>

class ZMethodNode;
class ZContextScope;
class ZVarsNode;

class ZClassNode : public ZScriptNode
{
public:

	void SetName(std::string name);
	void AddMethod(ZMethodNode* node);
	void SetMethods(std::vector<ZMethodNode*> methods);
	std::vector<ZMethodNode*> GetMethods();
	ZClassNode* CreateInstance(std::string name);
	std::string GetName();
	void AddVars(ZVarsNode* vars);

private:
	
	std::string mClassName;
	std::string mInherits;

	//meths
	std::vector<ZMethodNode*> mMethods;
	ZContextScope* mInstanceScope;

	std::vector<ZVarsNode*> mVars;

};

