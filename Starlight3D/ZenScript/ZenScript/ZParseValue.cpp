#include "ZParseValue.h"


ZParseValue::ZParseValue(ZTokenStream* stream) : ZParseNode(stream)
{

}

ZScriptNode* ZParseValue::Parse() {

	auto token = mStream->NextToken();

	int aa = 5;

	return nullptr;

}