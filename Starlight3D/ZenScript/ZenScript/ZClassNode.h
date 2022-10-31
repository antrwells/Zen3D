#pragma once
#include "ZScriptNode.h"
#include <string>
#include <vector>
#include <map>
#include <initializer_list>
#include "ZContextScope.h"

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
	void AddMethods(std::vector<ZMethodNode*> methods);
	std::vector<ZMethodNode*> GetMethods();
	ZClassNode* CreateInstance(std::string name, const std::vector<ZContextVar*>& params);
	void SetExtends(std::string name);
	std::string GetName();
	void AddVars(ZVarsNode* vars);
	std::vector<ZContextVar*> GetVars();
	void CreateScope();
	void PopulateScope();
	void SetVars(std::vector<ZVarsNode*> vars);
	void AddVars(std::vector<ZVarsNode*> vars);
	std::vector<ZVarsNode*> GetVarsVec() {
		return mVars;
	}
	//ZMethodNode* FindMethod(std::string name);
	ZMethodNode* FindMethod(size_t hash);
	ZContextVar* FindVar(std::string name);
	ZContextVar* FindVar(size_t name);
	ZContextVar* CallMethod(std::string name, const std::vector<ZContextVar*>& params);
	ZContextVar* CallMethod(size_t name, const std::vector<ZContextVar*>& params);
	void SetBaseName(std::string name);
	void Bind();
	std::string GetBaseName() {
		return mBaseClassName;
	}

	ZMethodNode* GetMethod(std::string name);
	ZMethodNode* GetMethod(size_t hash);

	ZContextScope* GetScope()
	{
		return mInstanceScope;
	}
	ZContextVar* FindComparer() {

		auto vars = mInstanceScope->GetVars();
		for (int i = 0; i < vars.size(); i++) {
			if (vars[i]->GetCompare())
			{
				return vars[i];
			}
		}
		return nullptr;
	}
	size_t GetHashName() {
		return mHashName;
	}
private:
	
	std::string mBaseClassName;
	std::string mClassName;
	std::string mInherits = "";

	//meths
	std::vector<ZMethodNode*> mMethods;
	ZContextScope* mInstanceScope;
	size_t mHashName;
	std::vector<ZVarsNode*> mVars;

};

