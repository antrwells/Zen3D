#pragma once
#include "ZScriptNode.h"
#include <string>
#include <vector>
#include <map>
#include <initializer_list>


class ZMethodNode;
class ZContextScope;
class ZVarsNode;
class ZContextVar;

class ZClassNode : public ZScriptNode
{
public:

	void SetName(std::string name);
	void AddMethod(ZMethodNode* node);
	void SetMethods(std::vector<ZMethodNode*> methods);
	std::vector<ZMethodNode*> GetMethods();
	ZClassNode* CreateInstance(std::string name, const std::vector<ZContextVar*>& params);
	std::string GetName();
	void AddVars(ZVarsNode* vars);
	std::vector<ZContextVar*> GetVars();
	void CreateScope();
	void PopulateScope();
	void SetVars(std::vector<ZVarsNode*> vars);
	ZMethodNode* FindMethod(std::string name);
	ZContextVar* FindVar(std::string name);
	ZContextVar* CallMethod(std::string name, const std::vector<ZContextVar*>& params);
	void SetBaseName(std::string name);
	void Bind();
	std::string GetBaseName() {
		return mBaseClassName;
	}
	ZContextScope* GetScope()
	{
		return mInstanceScope;
	}

private:
	
	std::string mBaseClassName;
	std::string mClassName;
	std::string mInherits;

	//meths
	std::vector<ZMethodNode*> mMethods;
	ZContextScope* mInstanceScope;

	std::vector<ZVarsNode*> mVars;

};

