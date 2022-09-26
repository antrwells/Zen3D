#include "ZParseExpression.h"
#include "ZExpressionNode.h"

ZParseExpression::ZParseExpression(ZTokenStream* stream) : ZParseNode(stream) {


}

ZScriptNode* ZParseExpression::Parse() {

	//auto token = mStream->NextToken();

	auto exp_node = new ZExpressionNode;

	Expression expr;

	while (!mStream->EOS()) {

		auto token = mStream->NextToken();
		ExpressionElement ele;

		int iVal;
		float fVal;

		switch (token.mType) {
		case TokenType::TokenMultiply:

			ele.mType = ExprElementType::EOp;
			ele.mOp = ExprOperatorType::OpMultiply;
			expr.mElements.push_back(ele);

			break;
		case TokenType::TokenDivide:

			ele.mType = ExprElementType::EOp;
			ele.mOp = ExprOperatorType::OpDivide;
			expr.mElements.push_back(ele);

			break;
		case TokenType::TokenMinus:

			ele.mType = ExprElementType::EOp;
			ele.mOp = ExprOperatorType::OpMinus;
			expr.mElements.push_back(ele);

			break;


		case TokenType::TokenPlus:

			ele.mType = ExprElementType::EOp;
			ele.mOp = ExprOperatorType::OpPlus;
			expr.mElements.push_back(ele);

			break;
		case TokenType::TokenFloat:
		
			ele.mType = ExprElementType::EFloat;
			ele.mValInt = 0;
			fVal = std::stof(std::string(token.mText));
			ele.mValFloat = fVal;
			expr.mElements.push_back(ele);

			break;
		case TokenType::TokenInt:

			ele.mType = ExprElementType::EInt;
			ele.mValInt = 0;
			iVal = std::stoi(std::string(token.mText));
			ele.mValInt = iVal;
			expr.mElements.push_back(ele);

			break;
		case TokenType::TokenString:
			break;
		case TokenType::TokenRightPara:
		case TokenType::TokenComma:
			mStream->Back();
			exp_node->SetExpression(expr);
			return exp_node;

			break;
		}

	}

	int aa = 5;

	return exp_node;
}