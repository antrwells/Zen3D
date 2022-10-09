#pragma once
#include <string>

//#include "DataTypes.h"
enum VarType;

class ZClassNode;

class ZContextVar
{
public:

	ZContextVar(std::string name,enum VarType type);

	std::string GetName();
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

};

