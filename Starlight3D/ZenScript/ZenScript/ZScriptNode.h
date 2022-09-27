#pragma once
#include <vector>
#include <initializer_list>

class ZContextVar;

class ZScriptNode
{
public:

	virtual ZContextVar* Exec(std::initializer_list<ZContextVar*> args);

public:
	
	std::vector<ZScriptNode*> mNodes;

};

