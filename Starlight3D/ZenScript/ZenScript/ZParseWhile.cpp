#include "ZParseWhile.h"
#include "ZWhileNode.h"
#include "ZParseExpression.h"
#include "ZExpressionNode.h"
#include "ZParseCodeBody.h"
#include "ZCodeBodyNode.h"
#include <assert.h>

ZParseWhile::ZParseWhile(ZTokenStream* stream) : ZParseNode(stream) {


}

ZScriptNode* ZParseWhile::Parse() {

	auto tok = mStream->NextToken();

	if (tok.mType != TokenType::TokenWhile) {
		assert(0);
	}

	ZWhileNode* node = new ZWhileNode;

	auto parse_expr = new ZParseExpression(mStream);
	auto expr_node = (ZExpressionNode*)parse_expr->Parse();

	node->SetExpression(expr_node);

	auto nt = mStream->NextToken();

	auto code_parse = new ZParseCodeBody(mStream);
	auto code_node = (ZCodeBodyNode*)code_parse->Parse();

	node->SetCode(code_node);

	return node;

}