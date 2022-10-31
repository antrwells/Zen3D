#include "ZParseExpression.h"
#include "ZExpressionNode.h"
#include "ZParseStatement.h"
#include "ZStatementNode.h"
#include "ZParseClassStatement.h"
#include "ZClassStatementNode.h"
#include "ZParseNew.h"

ZParseExpression::ZParseExpression(ZTokenStream* stream) : ZParseNode(stream) {


}

ZScriptNode* ZParseExpression::Parse() {

	//auto token = mStream->NextToken();

	auto exp_node = new ZExpressionNode;

	auto lb = ExpressionElement();
	lb.mOp = ExprOperatorType::OpLeftBrace;
	lb.mType = ExprElementType::EOp;


	Expression expr;
//	expr.mElements.push_back(lb);
	
	int v_id = 0;

	if (mStream->PeekToken(0).mType == TokenType::TokenEquals)
	{
		mStream->NextToken();
	}

	while (!mStream->EOS()) {

		auto token = mStream->NextToken();
		ExpressionElement ele;

		int iVal;
		float fVal;

		for (int i = 0; i < 16; i++) {
			ele.mValName[i] = "";
			ele.mNameHash[i] = 0;
		}

		switch (token.mType) {

		case TokenType::TokenUMinus:
			ele.mType = ExprElementType::EOp;
			ele.mOp = ExprOperatorType::OpUMinus;
			expr.mElements.push_back(ele);
			break;
		case TokenType::TokenAnd:
			ele.mType = ExprElementType::EOp;
			ele.mOp = ExprOperatorType::OpAnd;
			expr.mElements.push_back(ele);
			break;
		case TokenType::TokenOr:
			ele.mType = ExprElementType::EOp;
			ele.mOp = ExprOperatorType::OpOr;
			expr.mElements.push_back(ele);
			break;
			// ------ NOT
		case TokenType::TokenNotSame:
			ele.mType = ExprElementType::EOp;
			ele.mOp = ExprOperatorType::OpNot;
			expr.mElements.push_back(ele);
			break;

		case TokenType::TokenSame:

			ele.mType = ExprElementType::EOp;
			ele.mOp = ExprOperatorType::OpEquals;
			expr.mElements.push_back(ele);
			break;
		case TokenType::TokenLess:
			ele.mType = ExprElementType::EOp;
			ele.mOp = ExprOperatorType::OpLess;
			expr.mElements.push_back(ele);

			break;
		case TokenType::TokenGreater:
			ele.mType = ExprElementType::EOp;
			ele.mOp = ExprOperatorType::OpGreater;
			expr.mElements.push_back(ele);
			break;
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
		case TokenType::TokenNew:
			ele.mType = ExprElementType::ENew;
			ele.mValString = std::string(token.mText);
			
			{

				auto parse_new = new ZParseNew(mStream);
				auto new_node = (ZNewNode*)parse_new->Parse();
				ele.mNew = new_node;
			}

			expr.mElements.push_back(ele);
			exp_node->SetExpression(expr);
			return exp_node;


			break;
		case TokenType::TokenIdent:
		{
			std::hash<std::string> hasher;
			ele.mType = EVar;
			ele.mValName[0] = token.mText;
			ele.mNameHash[0] = hasher(token.mText);
		
			if (mStream->PeekToken(0).mType == TokenType::TokenPeriod) {

				mStream->NextToken();
				//ele.mValName[1] = mStream->NextToken().mText;

				ele.mNameHash[1] = hasher(mStream->NextToken().mText);
				if (mStream->PeekToken(0).mType == TokenType::TokenLeftPara)
				{
					mStream->Back();
					mStream->Back();
					mStream->Back();
					auto parse_classstate = new ZParseClassStatement(mStream);
					auto classstate_node = (ZClassStatementNode*)parse_classstate->Parse();
					ele.mClassStatement = classstate_node;
					ele.mType = EClassStatement;
					int yes = 1;
				}

			}
			else {

				auto vt = mStream->PeekToken(0);
				if (vt.mType == TokenType::TokenLeftPara) {

					mStream->Back();
					auto parse_state = new ZParseStatement(mStream);
					auto state_node = (ZStatementNode*)parse_state->Parse();
					ele.mStatement = state_node;
					ele.mType = EStatement;


				}
				int aa = 5;

			}

			expr.mElements.push_back(ele);

			//Error("tokenident");

			int av = 5;

			/*
			int nid = 0;
			ele.mType = ExprElementType::EVar;
			ele.mValName[nid] = token.mText;
			bool skip = false;
			if (mStream->PeekToken(0).mType == TokenType::TokenPeriod)
			{
				skip = false;
				int peek = 0;
				while (true) {
					auto ch = mStream->PeekToken(peek);
					if (ch.mType == TokenType::TokenLeftPara)
					{
						skip = true;
						break;
					}
					bool early = false;
					switch (ch.mType) {
					case TokenType::TokenPlus:
					case TokenType::TokenMinus:
					case TokenType::TokenMultiply:
					case TokenType::TokenDivide:
					case TokenType::TokenGreater:
					case TokenType::TokenLess:
					case TokenType::TokenComma:
						early = true;
						break;
					}

					if (early) {
						break;
					}

					if (ch.mType == TokenType::TokenEndOfLine)
					{
						break;
					}
					peek++;
				}
				if (!skip) {
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
			}
			if (skip && mStream->PeekToken(0).mType == TokenType::TokenPeriod) {

				mStream->Back();
				auto parse_classstate = new ZParseClassStatement(mStream);
				auto classstate_node = (ZClassStatementNode*)parse_classstate->Parse();
				ele.mClassStatement = classstate_node;
				ele.mType = EClassStatement;


			}else 
			if (mStream->PeekToken(0).mType == TokenType::TokenLeftPara)
			{
				mStream->Back();
				auto parse_state = new ZParseStatement(mStream);
				auto state_node = (ZStatementNode*)parse_state->Parse();
				ele.mStatement = state_node;
				ele.mType = EStatement;

				int bb = 5;
			}
			expr.mElements.push_back(ele);
			*/
		}

		break;
		case TokenType::TokenLeftPara:
			lb.mOp = ExprOperatorType::OpLeftBrace;
			expr.mElements.push_back(lb);
			break;
		case TokenType::TokenRightPara:

			if (mStream->PeekToken(0).mType == TokenType::TokenEndOfLine || mStream->PeekToken(0).mType == TokenType::TokenComma)
			{
				lb.mOp = ExprOperatorType::OpRightBrace;
				//expr.mElements.push_back(lb);
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
			//expr.mElements.push_back(lb);
			mStream->Back();
			exp_node->SetExpression(expr);
			return exp_node;

			break;
		
		default:
			lb.mOp = ExprOperatorType::OpRightBrace;
		//	expr.mElements.push_back(lb);
			mStream->Back();
			exp_node->SetExpression(expr);
			return exp_node;

		}

	}

	int aa = 5;

	return exp_node;
}