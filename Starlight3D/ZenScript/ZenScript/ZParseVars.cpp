#include "ZParseVars.h"
#include "ZVarsNode.h"
#include "ZExpressionNode.h"
#include "ZParseExpression.h"

ZParseVars::ZParseVars(ZTokenStream* stream) : ZParseNode(stream) {

}

ZScriptNode* ZParseVars::Parse() {

	auto var_type = mStream->NextToken();

	auto vars_node = new ZVarsNode;

	switch (var_type.mType)
	{
	case TokenType::TokenInt:
		vars_node->SetType(VarType::VarInt);
		break;
	case TokenType::TokenFloat:
		vars_node->SetType(VarType::VarFloat);
		break;
	case TokenType::TokenString:
		vars_node->SetType(VarType::VarString);
		break;
	}

	while (!mStream->EOS()) {

		auto tok = mStream->NextToken();

		auto def = new ZExpressionNode;

		if (tok.mType == TokenType::TokenEndOfLine)
		{
			return vars_node;
		}
		if (tok.mType == TokenType::TokenComma)
		{
			continue;
		}
		if (tok.mType == TokenType::TokenEquals)
		{

			

		}
		else {

			if (mStream->PeekToken(0).mType == TokenType::TokenEquals) {
				int cc = 1;
				auto exp_parse = new ZParseExpression(mStream);
				def = (ZExpressionNode*)exp_parse->Parse();
				int b = 5;
			}

			vars_node->AddVar(tok.mText,def);
		}
		int aa = 5;

	}

	//int b = 5;


	return nullptr;
}