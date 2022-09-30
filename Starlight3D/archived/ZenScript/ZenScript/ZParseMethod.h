#pragma once
#include "ZParseNode.h"
class ZParseMethod : ZParseNode
{
public:

	ZParseMethod(ZTokenStream* stream);
	ZScriptNode* Parse();

private:
};

