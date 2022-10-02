#pragma once
#include <chaiscript/chaiscript.hpp>
#include <string>

class ScriptObject;

class ScriptHost
{
public:

	ScriptHost();
	ScriptObject* CompileScriptText(std::string text);
	ScriptObject* CompileScriptFile(std::string path);
	void AddFunction(chaiscript::Proxy_Function func, std::string name);

private:
	chaiscript::ChaiScript host;
};

