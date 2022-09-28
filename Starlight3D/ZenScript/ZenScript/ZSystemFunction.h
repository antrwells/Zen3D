#pragma once
#include <string>
#include "ZContextVar.h"
#include <vector>
class ZSystemFunction
{
public:

	ZSystemFunction() {};
	ZSystemFunction(std::string name, ZContextVar* (*func)(const std::vector<ZContextVar*>& params));
	ZContextVar* CallFunction(const std::vector<ZContextVar*>& params);
	std::string GetName();

private:

	std::string mName;
	ZContextVar* (*mFunc)(const std::vector<ZContextVar*>& params);

};

