#pragma once

#include <string>


class ScriptObject;

class ScriptHost
{
public:

	ScriptHost();
	void LoadFile(const char* file);


	static ScriptHost* ScHost;
private:
	static bool Inited;
	

};

