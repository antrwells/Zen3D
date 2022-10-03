#include "ZParseExpression.h"
#include "ZExpressionNode.h"

ZParseExpression::ZParseExpression(ZTokenStream* stream) : ZParseNode(stream) {


}

ZScriptNode* ZParseExpression::Parse() {

	//auto token = mStream->NextToken();

	auto exp_node = new ZExpressionNode;

	auto lb = ExpressionElement();
	lb.mOp = ExprOperatorType::OpLeftBrace;
	lb.mType = ExprElementType::EOp;


	Expression expr;
	expr.mElements.push_back(lb);
	
	int v_id = 0;

	while (!mStream->EOS()) {

		auto token = mStream->NextToken();
		ExpressionElement ele;

		int iVal;
		float fVal;

		for (int i = 0; i < 16; i++) {
			ele.mValName[i] = "";
		}

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

			ele.mType = ExprElementType::EString;
			ele.mValString = std::string(token.mText);
			expr.mElements.push_back(ele);

			break;
		case TokenType::TokenIdent:
		{
			int nid = 0;
			ele.mType = ExprElementType::EVar;
			ele.mValName[nid] = token.mText;

			if (mStream->PeekToken(0).mType == TokenType::TokenPeriod)
			{
				while (true) {
					token = mStream->NextToken();
					if (token.mType == TokenType::TokenPeriod)
					{
						nid++;
						ele.mValName[nid] = mStream->NextToken().mText;
					}
					else {
						int cc = 5;
						mStream->Back();
						break;
					}
				}
			}
			expr.mElements.push_back(ele);
		}

			break;
		case TokenType::TokenRightPara:

			if (mStream->PeekToken(0).mType == TokenType::TokenEndOfLine)
			{
				lb.mOp = ExprOperatorType::OpRightBrace;
				expr.mElements.push_back(lb);
				mStream->Back();
				exp_node->SetExpression(expr);
				return exp_node;
			}
			else {
				lb.mOp = ExprOperatorType::OpRightBrace;
				expr.mElements.push_back(lb);
			}

			break;
		case TokenType::TokenComma:
		case TokenType::TokenEndOfLine:

			lb.mOp = ExprOperatorType::OpRightBrace;
			expr.mElements.push_back(lb);
			mStream->Back();
			exp_node->SetExpression(expr);
			return exp_node;

			break;
		}

	}

	int aa = 5;

	return exp_node;
}