#pragma once
#include <string>

//#include "DataTypes.h"
enum VarType;

class ZClassNode;

class ZContextVar
{
public:

	ZContextVar(std::string name,enum VarType type,std::string baseID,bool comparer);

	std::string GetName();
	size_t GetHashName();
	VarType GetType();
	int GetIntVal();
	float GetFloatVal();
	std::string GetStringVal();
	ZClassNode* GetClassVal();
	void SetInt(int val);
	void SetFloat(float val);
	void SetString(std::string val);
	void SetClass(ZClassNode* cls);
	void SetCObj(void* obj);
	void* GetCObj();
	int GetTypeInt() {
		return (int)mType;
	}
	std::string GetBaseID() {
		return mBaseID;
	}
	void SetBaseID(std::string name) {
		mBaseID = name;
	}
	bool GetCompare() {
		return mComparer;
	}

	//For run/stop type scenarios.

	void Push();
	void Pop();

private:

	std::string mName;
	VarType mType;
	int mIntVal;
	float mFloatVal;
	std::string mStringVal;
	ZClassNode* mClassVal;
	void* mCObj;
	float mPushFloat;
	int mPushInt;
	std::string mPushString;
	std::string mBaseID;
	bool mComparer = false;
	size_t mHashName;

};


ZContextVar* VMakeInt(int v,bool comparer = false);
ZContextVar* VMakeFloat(float v,bool cmparer = false);
ZContextVar* VMakeString(std::string v,bool comparer=false);
ZContextVar* VMakeC(void* v,bool comparer=false);
ZContextVar* VMakeClass(ZClassNode* v,bool comparer=false);

int VGetInt(ZContextVar* v);
float VGetFloat(ZContextVar* v);
std::string VGetString(ZContextVar* v);
void* VGetC(ZContextVar* v);
ZClassNode* VGetClass(ZContextVar* v);