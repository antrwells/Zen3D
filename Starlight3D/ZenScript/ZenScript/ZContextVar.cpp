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

	if (mType == VarType::VarFloat)
	{
		return (int)mFloatVal;
	}
	return mIntVal;

}

float ZContextVar::GetFloatVal() {

	if (mType == VarType::VarInt)
	{
		return (float)mIntVal;
	}
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

	if (mType == VarType::VarFloat) {
		mFloatVal = (float)val;
	}
	mIntVal = val;

}

void ZContextVar::SetFloat(float val) {

	if (mType == VarType::VarInt)
	{
		mIntVal = (int)val;
	}
	mFloatVal = val;

}

void ZContextVar::SetString(std::string val)
{

	mStringVal = val;

}

void ZContextVar::SetClass(ZClassNode* cls) {

	mClassVal = cls;

}

void* ZContextVar::GetCObj() {

	return mCObj;

}

void ZContextVar::SetCObj(void * obj) {

	mCObj = obj;

}