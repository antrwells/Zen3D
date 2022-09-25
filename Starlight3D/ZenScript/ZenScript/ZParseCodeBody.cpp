#include "ZParseCodeBody.h"
#include "ZParseStatement.h"
#include "ZStatementNode.h"
#include "ZCodeBodyNode.h"
ZParseCodeBody::ZParseCodeBody(ZTokenStream* stream) : ZParseNode(stream) {


}

CodeType ZParseCodeBody::PredictType() {

	//auto token = mStream->NextToken();


	int peek_val = 0;

	while (!mStream->EOS())
	{

		auto token = mStream->PeekToken(peek_val);

		switch (token.mType) {
		case TokenType::TokenLeftPara:

			return CodeType::CodeStatement;

			break;
		}

		peek_val++;

	}

	int aa = 5;

	return CodeType::CodeUnknown;

}

ZScriptNode* ZParseCodeBody::Parse() {

	//auto tok = mStream->NextToken();

	auto codebody = new ZCodeBodyNode;

	while (!mStream->EOS()) {

		auto code_type = PredictType();

		ZParseStatement* parse_statement = nullptr;
		ZStatementNode* statement_node = nullptr;

		switch (code_type) {
		case CodeType::CodeStatement:

			parse_statement = new ZParseStatement(mStream);

			statement_node = (ZStatementNode*)parse_statement->Parse();

			codebody->AddNode(statement_node);

			break;
		}

		int bb = 5;

	}

	int aa = 5;


	return nullptr;

}