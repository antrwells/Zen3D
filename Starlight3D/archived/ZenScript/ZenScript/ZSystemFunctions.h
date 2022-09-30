#pragma once
#include <string>
#include <vector>

class ZSystemFunction;
class ZContextVar;

class ZSystemFunctions
{
public:

	ZSystemFunctions();
	void RegisterFunction(ZSystemFunction function);
	ZContextVar* CallFunction(std::string name, const std::vector<ZContextVar*>& params);
	static ZSystemFunctions* sFuncs;

private:

	std::vector<ZSystemFunction> mFunctions;

};

