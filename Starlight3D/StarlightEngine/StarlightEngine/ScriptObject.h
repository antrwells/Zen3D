#pragma once
#include <string>
#include <vector>

class ZScriptContext;
class ZClassNode;
class ZContextVar;

class ScriptObject
{
public:
	ZScriptContext* mContext = nullptr;
	ZClassNode* mMainClass = nullptr;
	std::string name = "";
	void CallUpdate();
	void CallInit();
	void CallUI();
	void CallRender();
	std::vector<ZContextVar*> GetVars();

};

