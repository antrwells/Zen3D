#include "ZParseReturn.h"
#include <assert.h>
#include "ZParseExpression.h"
#include "ZExpressionNode.h"
#include "ZReturnNode.h"
ZParseReturn::ZParseReturn(ZTokenStream* stream) : ZParseNode(stream) {

}

ZScriptNode* ZParseReturn::Parse() {

	auto tok = mStream->NextToken();
	if (tok.mType == TokenType::TokenEndOfLine) {
		tok = mStream->NextToken();
	}
	if (tok.mType != TokenType::TokenReturn)
	{
		assert(0);
	}

	auto ret_expr = new ZParseExpression(mStream);
	auto expr_node = (ZExpressionNode*)ret_expr->Parse();

	auto ret = new ZReturnNode;
	ret->SetExpression(expr_node);
	ret->SetReturn(true);

	int p = 0;

	return ret;
}