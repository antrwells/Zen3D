#pragma once
#include "sol.hpp"
class ScriptObject
{
public:
	sol::load_result s;
	sol::state state;
	std::string name;
};

