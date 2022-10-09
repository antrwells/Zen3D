#pragma once
#include "ZProgramNode.h"
#include <string>
#include <stack>
//#include "DataTypes.h"

#include "ZContextVar.h"
class ZContextScope;
class ZSystemFunctions;

class ZScriptContext
{
public:

	ZScriptContext();
	void SetupSystem();
	void AddNode(ZMainNode* node);
	ZClassNode* CreateInstance(std::string name,std::string instance_name, const std::vector<ZContextVar*>& params);
	void PushClass(ZClassNode* cls);
	void PopClass();
	ZClassNode* GetClass();
	ZContextVar* CallMethod(std::string name, std::string meth, const std::vector<ZContextVar*>& params);
	ZClassNode* FindInstance(std::string name);
	static ZContextScope* CurrentScope;
	static ZScriptContext* CurrentContext;
	void PushScope(ZContextScope* scope)
	{
		mScope.push(scope);
	}
	void PopScope() {
		mScope.pop();
	}
	ZContextScope* GetScope()
	{
		return mScope.top();
	}
	ZContextVar* RunLine(std::string code);

	void LoadLib(std::string name);
	ZSystemFunctions* GetSysFuncs();
private:

	ZSystemFunctions* mSysFuncs;
	std::vector<ZClassNode*> mClasses;
	std::stack<ZClassNode*> mClassStack;
	std::vector<ZClassNode*> mInstances;
	std::stack<ZContextScope*> mScope;

};

