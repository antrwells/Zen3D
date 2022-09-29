#pragma once
#include "ZProgramNode.h"
#include <string>
#include <stack>
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
	void PushClass(ZClassNode* cls);
	void PopClass();
	ZClassNode* GetClass();

	static ZContextScope* CurrentScope;
	static ZScriptContext* CurrentContext;
	

private:

	std::vector<ZClassNode*> mClasses;
	std::stack<ZClassNode*> mClassStack;

};

