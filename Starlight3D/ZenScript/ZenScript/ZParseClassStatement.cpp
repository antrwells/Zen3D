#include "ZParseClassStatement.h"
#include "ZClassStatementNode.h"
#include "ZParseParameters.h"
#include "ZParametersNode.h"
ZParseClassStatement::ZParseClassStatement(ZTokenStream* stream) : ZParseNode(stream) {

}

ZScriptNode* ZParseClassStatement::Parse() {

//	auto tok = mStream->NextToken();

	auto cs_node = new ZClassStatementNode;
	if (mStream->PeekToken(0).mType == TokenType::TokenEndOfLine) {
		mStream->NextToken();
	}


	while (!mStream->EOS()) {

		auto tok = mStream->NextToken();

		ZParseParameters* parse_pars = nullptr;
		ZParametersNode* pars_node = nullptr;

		switch (tok.mType)
		{
		case TokenType::TokenIdent:
			//mNames.push_back(tok.mText);
			cs_node->AddName(tok.mText);
			break;
		case TokenType::TokenPeriod:
			continue;
			break;
		case TokenType::TokenLeftPara:

			mStream->Back();

			parse_pars = new ZParseParameters(mStream);

			pars_node = (ZParametersNode*)parse_pars->Parse();

			cs_node->SetParameters(pars_node);

			mStream->Back();


			break;
		case TokenType::TokenRightPara:
		case TokenType::TokenEndOfLine:

			return cs_node;
		}

	}

	int bb = 5;

	return nullptr;

}