#include "ZParseParameters.h"
#include <assert.h>
#include "ZParseExpression.h"
#include "ZExpressionNode.h"
#include "ZParametersNode.h"

ZParseParameters::ZParseParameters(ZTokenStream* stream) : ZParseNode(stream) {

}

ZScriptNode* ZParseParameters::Parse() {

	auto check = mStream->NextToken();

	if (check.mType != TokenType::TokenLeftPara)
	{
		/// <summary>
		/// *ERROR*
		/// </summary>
		/// <returns></returns>
		assert(false);
	}

	auto par_node = new ZParametersNode;

	while (!mStream->EOS()) {

		auto token = mStream->NextToken();

		ZParseExpression* parse_exp;
		ZExpressionNode* exp_node;

		switch (token.mType)
		{
		case TokenType::TokenComma:
			continue;
			break;
		case TokenType::TokenRightPara:
		case TokenType::TokenEndOfLine:
			return par_node;

			break;
		
		default:

			mStream->Back();

			parse_exp = new ZParseExpression(mStream);

			exp_node = (ZExpressionNode*)parse_exp->Parse();

			par_node->AddExpression(exp_node);

			int aa = 5;
			break;
		}

	}

	//int aa = 5;
	assert(false);
	return nullptr;
}