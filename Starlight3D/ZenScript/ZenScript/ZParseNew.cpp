#include "ZParseNew.h"
#include "ZNewNode.h"
#include "ZParseParameters.h"
#include "ZParametersNode.h"
#include <assert.h>
ZParseNew::ZParseNew(ZTokenStream* stream) : ZParseNode(stream) {

}

ZScriptNode* ZParseNew::Parse() {

	auto tok = mStream->NextToken();

	if (tok.mType == TokenType::TokenEquals)
	{

	}
	else {
		mStream->Back();
	}

	tok = mStream->NextToken();

	if (tok.mType != TokenType::TokenNew)
	{
		mStream->Back();
	
		//assert(false);
	}

	tok = mStream->NextToken();

	int aa = 5;

	auto parse_node = new ZNewNode;

	parse_node->SetClassName(tok.mText);

	auto pars = new ZParseParameters(mStream);

	auto pars_node = (ZParametersNode*)pars->Parse();

	parse_node->SetParameters(pars_node);

	int a = 5;


	return parse_node;
}