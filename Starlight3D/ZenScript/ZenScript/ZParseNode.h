#pragma once
#include "ZTokenStream.h"
#include "ZScriptNode.h"
class ZParseNode
{
public:
	
	ZParseNode(ZTokenStream* stream);
	virtual ZScriptNode* Parse();

	void Error(const char* err);
	int TokeStart = 0;
private:

protected:
	ZTokenStream *mStream;

};

