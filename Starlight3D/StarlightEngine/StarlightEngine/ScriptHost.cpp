#include "pch.h"
#include "ScriptHost.h"

ScriptHost::ScriptHost() {

}

ScriptObject* ScriptHost::CompileScriptText(std::string code)
{

	host.eval(code);

	return nullptr;
}

ScriptObject* ScriptHost::CompileScriptFile(std::string path)
{

	host.eval_file(path);

	return nullptr;

}

void ScriptHost::AddFunction(chaiscript::Proxy_Function func, std::string name) {

	host.add(func, name);

}