#pragma once
#include "ZParseNode.h"

class ZParseClass : public ZParseNode
{
public:
	ZParseClass(ZTokenStream* stream);
	ZScriptNode* Parse();

};

