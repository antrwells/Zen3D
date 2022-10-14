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
	std::string mPath;
	std::string mClsName;
	void CallUpdate();
	void CallInit();
	void CallUI();
	void CallRender();
	void PushVars();
	void PopVars();
	std::vector<ZContextVar*> GetVars();

};

