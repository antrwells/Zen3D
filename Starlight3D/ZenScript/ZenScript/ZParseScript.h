#pragma once
#include "ZTokenStream.h"
#include "ZParseNode.h"
class ZParseScript : public ZParseNode
{
public:

	ZParseScript(ZTokenStream* stream);
	ZScriptNode* Parse();

	

private:
};

