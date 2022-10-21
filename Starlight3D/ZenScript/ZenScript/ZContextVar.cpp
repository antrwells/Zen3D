#include "ZContextVar.h"
//#include "ZClassNode.h"

#include "VarTypes.h"
#include "ZClassNode.h"

ZContextVar::ZContextVar(std::string name, enum VarType type,std::string baseID,bool comparer) {

	mName = name;
	mType = type;
	mIntVal = 0;
	mFloatVal = 0;
	mStringVal = "";
	mClassVal = nullptr;
	mBaseID = baseID;
	mComparer = comparer;
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

void ZContextVar::Push() {

	switch (mType)
	{
	case VarType::VarFloat:
		mPushFloat = mFloatVal;
		break;
	case VarType::VarInt:
		mPushInt = mIntVal;
		break;
	case VarType::VarInstance:

	{
		auto vc = mClassVal;
		if (vc == nullptr) return;
		auto vars = vc->GetVars();
		for (int i = 0; i < vars.size(); i++)
		{
			auto v = vars[i];
			v->Push();
		}
	}
		break;
	case VarType::VarString:
		mPushString = mStringVal;
		break;
	}

		

}

void ZContextVar::Pop() {

	switch (mType) {
	case VarType::VarFloat:

		mFloatVal = mPushFloat;

		break;
	case VarType::VarInt:
		mIntVal = mPushInt;
		break;
	case VarType::VarInstance:
	{
		auto vc = mClassVal;
		if (vc == nullptr) return;
		auto vars = vc->GetVars();
		for (int i = 0; i < vars.size(); i++)
		{
			auto v = vars[i];
			v->Pop();
		}
	}	break;
	case VarType::VarString:
		mStringVal = mPushString;
		break;
	}

}


ZContextVar* VMakeInt(int v,bool comparer)
{
	auto var = new ZContextVar("", VarType::VarInt,"int",comparer);
	var->SetInt(v);


	return var;

}
ZContextVar* VMakeFloat(float v,bool comparer)
{
	auto var = new ZContextVar("", VarType::VarFloat,"float",comparer);
	var->SetFloat(v);

	return var;

}

ZContextVar* VMakeString(std::string v,bool comparer)
{
	auto var = new ZContextVar("", VarType::VarString,"string",comparer);
	var->SetString(v);

	return var;

}

ZContextVar* VMakeC(void* v,bool comparer)
{
	auto var = new ZContextVar("", VarType::VarCObj,"CObj",comparer);
	var->SetCObj(v);

	return var;

}

ZContextVar* VMakeClass(ZClassNode* v,bool comparer)
{

	auto var = new ZContextVar("", VarType::VarInstance,v->GetBaseName(),comparer);
	var->SetClass(v);

	return var;

}

int VGetInt(ZContextVar* v)
{
	return v->GetIntVal();
}

float VGetFloat(ZContextVar* v)
{
	return v->GetFloatVal();
}

std::string VGetString(ZContextVar* v)
{
	return v->GetStringVal();
}

ZClassNode* VGetClass(ZContextVar* v)
{
	return v->GetClassVal();
}

void* VGetC(ZContextVar* v)
{
	return v->GetCObj();
}


