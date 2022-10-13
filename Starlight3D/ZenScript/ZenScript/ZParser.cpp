#include "ZParser.h"
#include "ZParseScript.h"
#include "ZProgramNode.h"

ZParser::ZParser(ZTokenStream* stream) {

	mStream = stream;



}

ZMainNode* ZParser::Parse()
{

	ZParseScript* parseScript = new ZParseScript(mStream);

	return (ZMainNode*)parseScript->Parse();


}

