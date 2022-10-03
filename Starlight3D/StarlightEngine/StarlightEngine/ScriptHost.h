#pragma once

#include <string>
#include "sol.hpp"

class ScriptObject;

class ScriptHost
{
public:

	ScriptHost();
	void RunFile(const char* file);
	void SetFunction(void* func, std::string name);

	static ScriptHost* ScHost;
private:
	static bool Inited;
	sol::state host;

};

