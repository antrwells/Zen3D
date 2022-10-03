#include "ZSystemFunction.h"


ZSystemFunction::ZSystemFunction(std::string name, ZContextVar* (*func)(const std::vector<ZContextVar*>& params))
{

	mName = name;
	mFunc = func;

}

ZContextVar* ZSystemFunction::CallFunction(const std::vector<ZContextVar*>& params)
{

	return mFunc(params);


	//return nullptr;
}

std::string ZSystemFunction::GetName() {

	return mName;

}