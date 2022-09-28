#pragma once
#include <vector>
#include <initializer_list>

class ZContextVar;

class ZScriptNode
{
public:

	virtual ZContextVar* Exec(const std::vector<ZContextVar*>& params);

public:
	
	std::vector<ZScriptNode*> mNodes;

};

