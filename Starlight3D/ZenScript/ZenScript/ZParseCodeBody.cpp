#include "ZParseCodeBody.h"
#include "ZParseStatement.h"
#include "ZParseVars.h"
#include "ZVarsNode.h"
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
		case TokenType::TokenInt:
		case TokenType::TokenFloat:
			return CodeType::CodeDeclareVars;
			break;

		case TokenType::TokenLeftPara:

			return CodeType::CodeStatement;

			break;
		case TokenType::TokenEnd:

			return CodeType::CodeEnd;

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

	Token tk(TokenType::EndOfFile, "");

	while (!mStream->EOS()) {

		auto code_type = PredictType();

		ZParseStatement* parse_statement = nullptr;
		ZStatementNode* statement_node = nullptr;
		ZParseVars* parse_vars = nullptr;
		ZVarsNode* vars_node = nullptr;

		int e = 0;
		switch (code_type) {
		case CodeType::CodeEnd:

			mStream->NextToken();
			mStream->AssertNextToken(TokenType::TokenEndOfLine);
			return codebody;

			break;
		case CodeType::CodeDeclareVars:

			parse_vars = new ZParseVars(mStream);
			vars_node = (ZVarsNode*)parse_vars->Parse();

			codebody->AddNode(vars_node);

			//tk = mStream->NextToken();
			vars_node->SetCodeOwner(codebody);

			e = 1;

			break;
		case CodeType::CodeStatement:

			parse_statement = new ZParseStatement(mStream);

			statement_node = (ZStatementNode*)parse_statement->Parse();

			codebody->AddNode(statement_node);

			auto next_tok = mStream->AssertNextToken(TokenType::TokenEndOfLine);

			break;
		}

		int bb = 5;

	}

	int aa = 5;


	return nullptr;

}