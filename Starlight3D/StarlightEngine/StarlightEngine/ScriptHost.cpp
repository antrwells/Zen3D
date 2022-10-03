#include "pch.h"
#include "ScriptHost.h"

ScriptHost::ScriptHost() {

//	if (!Inited) {
		
		host.open_libraries(sol::lib::base, sol::lib::package);



}

void ScriptHost::RunFile(const char* path)
{
	auto file = host.load_file(std::string(path));
	file();

}

void ScriptHost::SetFunction(void* func, std::string name) {
	
	host[name] = func;
	//host[name] = func;


}

ScriptHost* ScriptHost::ScHost = nullptr;

bool ScriptHost::Inited = false;