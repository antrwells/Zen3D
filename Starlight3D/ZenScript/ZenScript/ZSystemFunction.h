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
	size_t GetHashName()
	{
		return mHashName;
	}
	bool IsAlive() {
		return Alive;
	}
private:

	std::string mName;
	size_t mHashName;
	ZContextVar* (*mFunc)(const std::vector<ZContextVar*>& params);
	bool Alive = false;
};

