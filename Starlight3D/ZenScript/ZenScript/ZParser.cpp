#include "ZParser.h"
#include "ZParseScript.h"

ZParser::ZParser(ZTokenStream* stream) {

	mStream = stream;

}

void ZParser::Parse()
{

	ZParseScript* parseScript = new ZParseScript(mStream);

	parseScript->Parse();

}