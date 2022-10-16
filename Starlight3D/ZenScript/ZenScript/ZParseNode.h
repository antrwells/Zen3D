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
	void StopAt(std::string text) {

		if (mStream->PeekToken(0).mText == text) {
			int stop;

		}

	}
private:

protected:
	ZTokenStream *mStream;

};

