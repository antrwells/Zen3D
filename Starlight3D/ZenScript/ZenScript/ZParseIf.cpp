#include "ZParseIf.h"
#include <assert.h>
#include "ZExpressionNode.h"
#include "ZParseExpression.h"
#include "ZIfnode.h"
#include "ZParseCodeBody.h"

ZParseIf::ZParseIf(ZTokenStream* stream) : ZParseNode(stream) {

}

ZScriptNode* ZParseIf::Parse() {

	auto tok = mStream->NextToken();

	if(tok.mType != TokenType::TokenIf)
	{
		mStream->Back();
		if (mStream->PeekToken(0).mType != TokenType::TokenIf) {
			assert(0);
		}
	}

	auto parse_e = new ZParseExpression(mStream);
	auto e_node = (ZExpressionNode*)parse_e->Parse();
	int a = 5;

	auto if_node = new ZIfNode;
	if_node->SetIfExpression(e_node);

	auto parse_code = new ZParseCodeBody(mStream);
	auto true_code = (ZCodeBodyNode*)parse_code->Parse();

	if_node->SetTrueCode(true_code);

	mStream->Back();
	auto ct = mStream->NextToken();



	return if_node;

	return nullptr;
}