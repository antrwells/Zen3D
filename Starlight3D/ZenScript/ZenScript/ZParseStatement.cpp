#include "ZParseStatement.h"
#include "ZStatementNode.h"

ZParseStatement::ZParseStatement(ZTokenStream* stream) : ZParseNode(stream) {

}

ZScriptNode* ZParseStatement::Parse() {

	//auto token = mStream->NextToken();
	auto state_node = new ZStatementNode;

	while (!mStream->EOS()) {

		auto token = mStream->NextToken();

		switch (token.mType) {

		case TokenType::TokenIdent:

			state_node->AddCallName(token.mText);

			break;

		case TokenType::TokenLeftPara:

			mStream->Back();

			int aa = 5;

			break;

		}

		

	}


	//int aa = 5;


	return state_node;
}