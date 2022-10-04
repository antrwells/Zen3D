#include "ZParseFor.h"
#include "ZParseAssign.h"
#include "ZParseExpression.h"
#include "ZExpressionNode.h"
#include "ZForNode.h"
#include "ZParseCodeBody.h"
#include "ZCodeBodyNode.h"
ZParseFor::ZParseFor(ZTokenStream* stream) : ZParseNode(stream) {

}

ZScriptNode* ZParseFor::Parse() {

	auto tok = mStream->NextToken();
	if (tok.mType == TokenType::TokenEndOfLine) {
		mStream->NextToken();
	}
	auto parse_init = new ZParseAssign(mStream);
	auto init_node = (ZAssignNode*)parse_init->Parse();

	tok = mStream->NextToken();
	if (tok.mType != TokenType::TokenComma)
	{

		int bb = 5;
	}

	auto parse_until = new ZParseExpression(mStream);
	auto until_node = (ZExpressionNode*)parse_until->Parse();

	auto for_node = new ZForNode;
	for_node->SetInit(init_node);
	for_node->SetUntil(until_node);

	tok = mStream->NextToken();
	if (tok.mType != TokenType::TokenComma)
	{

		int bb = 5;
	}

	auto inc_parse = new ZParseAssign(mStream);
	auto inc_node = (ZAssignNode*)inc_parse->Parse();

	auto parse_code = new ZParseCodeBody(mStream);
	auto code_node = (ZCodeBodyNode*)parse_code->Parse();
	for_node->SetIncAssign(inc_node);
	for_node->SetCode(code_node);

	int aa = 5;

	return for_node;

}