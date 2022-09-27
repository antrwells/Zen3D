#include "ZContextVar.h"
#include "ZClassNode.h"
#include "VarTypes.h"

ZContextVar::ZContextVar(std::string name, enum VarType type) {

	mName = name;
	mType = type;
	mIntVal = 0;
	mFloatVal = 0;
	mStringVal = "";
	mClassVal = nullptr;

}

std::string ZContextVar::GetName() {

	return mName;

}

VarType ZContextVar::GetType() {

	return mType;

}

int ZContextVar::GetIntVal() {

	return mIntVal;

}

float ZContextVar::GetFloatVal() {

	return mFloatVal;

}

std::string ZContextVar::GetStringVal() {

	return mStringVal;

}


ZClassNode* ZContextVar::GetClassVal()
{

	return mClassVal;

}

void ZContextVar::SetInt(int val) {

	mIntVal = val;

}

void ZContextVar::SetFloat(float val) {

	mFloatVal = val;

}

void ZContextVar::SetString(std::string val)
{

	mStringVal = val;

}



