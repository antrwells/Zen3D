#pragma once
#include "ZTokenStream.h"
#include "ZScriptNode.h"
class ZParseNode
{
public:
	
	ZParseNode(ZTokenStream* stream);
	virtual ZScriptNode* Parse();


private:

protected:
	ZTokenStream *mStream;

};

