#pragma once
#include <vector>
#include <initializer_list>

class ZContextVar;
class ZClassNode;

class ZScriptNode
{
public:

	virtual ZContextVar* Exec(const std::vector<ZContextVar*>& params);
	ZClassNode* GetClassOwner() {
		return mClass;
	}
	void SetClassOwner(ZClassNode* cls)
	{
		mClass = cls;
	}
	virtual void Bind() {};
	bool IsReturn() {
		return mIsReturn;
	}
	void SetReturn(bool ret) {
		mIsReturn = true;
	}
	void SetDebug(bool debug) {
		mDebug = debug;
	}
	bool IsDebug() {
		return mDebug;
	}
public:
	
	std::vector<ZScriptNode*> mNodes;
	ZClassNode* mClass;
	bool mIsReturn = false;
	bool mDebug = false;

};

