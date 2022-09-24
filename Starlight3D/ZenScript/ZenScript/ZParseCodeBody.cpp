#include "ZParseCodeBody.h"


ZParseCodeBody::ZParseCodeBody(ZTokenStream* stream) : ZParseNode(stream) {


}

ZScriptNode* ZParseCodeBody::Parse() {

	auto tok = mStream->NextToken();

	int aa = 5;

	return nullptr;

}