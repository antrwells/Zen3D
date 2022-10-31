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
	ZContextVar* CallFunction(size_t name, const std::vector<ZContextVar*>& params);
	ZSystemFunction GetFunction(size_t name);
	static ZSystemFunctions* sFuncs;

private:

	std::vector<ZSystemFunction> mFunctions;

};

