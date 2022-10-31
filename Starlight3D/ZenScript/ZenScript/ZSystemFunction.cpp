#include "ZSystemFunction.h"


ZSystemFunction::ZSystemFunction(std::string name, ZContextVar* (*func)(const std::vector<ZContextVar*>& params))
{
	std::hash<std::string> hasher;
	mName = name;
	mFunc = func;
	mHashName = hasher(name);
	Alive = true;

}

ZContextVar* ZSystemFunction::CallFunction(const std::vector<ZContextVar*>& params)
{

	return mFunc(params);


	//return nullptr;
}

std::string ZSystemFunction::GetName() {

	return mName;

}