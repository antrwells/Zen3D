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


private:

	std::string mName;
	VarType mType;
	int mIntVal;
	float mFloatVal;
	std::string mStringVal;
	ZClassNode* mClassVal;

};

