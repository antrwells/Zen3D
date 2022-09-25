#include "ZParseStatement.h"
#include "ZStatementNode.h"
#include "ZParseParameters.h"
#include "ZParametersNode.h"

ZParseStatement::ZParseStatement(ZTokenStream* stream) : ZParseNode(stream) {

}

ZScriptNode* ZParseStatement::Parse() {

	//auto token = mStream->NextToken();
	auto state_node = new ZStatementNode;

	while (!mStream->EOS()) {

		auto token = mStream->NextToken();

		ZParseParameters* parse_pars = nullptr;
		ZParametersNode* pars_node = nullptr;

		switch (token.mType) {

		case TokenType::TokenIdent:

			state_node->AddCallName(token.mText);

			break;

		case TokenType::TokenLeftPara:

			mStream->Back();

			parse_pars = new ZParseParameters(mStream);

			pars_node = (ZParametersNode*)parse_pars->Parse();

			state_node->SetPars(pars_node);

			int aa = 5;

			break;

		}

		

	}


	//int aa = 5;


	return state_node;
}