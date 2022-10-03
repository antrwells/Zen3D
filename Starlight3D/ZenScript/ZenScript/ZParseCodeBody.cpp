#include "ZParseCodeBody.h"
#include "ZParseStatement.h"
#include "ZParseVars.h"
#include "ZVarsNode.h"
#include "ZStatementNode.h"
#include "ZCodeBodyNode.h"
#include "ZParseClassStatement.h"
#include "ZClassStatementNode.h"
#include "ZParseAssign.h"
#include "ZParseIf.h"
#include "ZIfnode.h"
ZParseCodeBody::ZParseCodeBody(ZTokenStream* stream) : ZParseNode(stream) {



}

CodeType ZParseCodeBody::PredictType() {

	//auto token = mStream->NextToken();


	int peek_val = 0;
	bool decvar = false;

	while (!mStream->EOS())
	{

		auto token = mStream->PeekToken(peek_val);
		int ee = 1;
		switch (token.mType) {
		case TokenType::TokenIf:

			return CodeType::CodeIf;

		case TokenType::TokenPeriod:

			if (mStream->FindInLine(TokenType::TokenLeftPara,peek_val))
			{
				return CodeType::ClassStatement;
			}
			break;
		case TokenType::TokenIdent:



			if (mStream->FindInLine(TokenType::TokenPeriod,peek_val))
			{
				peek_val++;
				continue;
			}

			if (mStream->FindInLine(TokenType::TokenEquals,peek_val))
			{
				if (mStream->FindInLine(TokenType::TokenNew,peek_val))
				{

					return CodeType::CodeDeclareVars;
				}
				return CodeType::CodeAssign;
			}
			else {
				return CodeType::CodeStatement;
			}

			break;

		case TokenType::TokenInt:
		case TokenType::TokenFloat:
			return CodeType::CodeDeclareVars;
			break;

		case TokenType::TokenLeftPara:

			return CodeType::CodeStatement;

			break;
		case TokenType::TokenEnd:
		case TokenType::TokenElse:
		case TokenType::TokenElseIf:

			return CodeType::CodeEnd;

			break;
		case TokenType::TokenEquals:

			return CodeType::CodeAssign;

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
		auto vv = mStream->PeekToken(0);
		ZParseStatement* parse_statement = nullptr;
		ZStatementNode* statement_node = nullptr;
		ZParseVars* parse_vars = nullptr;
		ZVarsNode* vars_node = nullptr;
		ZParseClassStatement* parse_clsstatement = nullptr;
		ZClassStatementNode* clsstatement_node = nullptr;
		Token ct(TokenType::TokenVoid, "");
		int e = 0;
		switch (code_type) {
		case CodeType::CodeIf:
		{

			auto parse_if = new ZParseIf(mStream);
			auto if_node = parse_if->Parse();
			codebody->AddNode(if_node);

		}

			break;
		case CodeType::CodeAssign:
		{
			auto tok = mStream->NextToken();
			int aa = 5;


			if (mStream->PeekToken(0).mType == TokenType::TokenEquals)
			{
				mStream->Back();
			}
			auto parse_ass = new ZParseAssign(mStream);
			auto parse_node = parse_ass->Parse();
			codebody->AddNode(parse_node);

		}

			break;
		case CodeType::ClassStatement:

			e = 1;
			parse_clsstatement = new ZParseClassStatement(mStream);
			clsstatement_node = (ZClassStatementNode*)parse_clsstatement->Parse();
			codebody->AddNode(clsstatement_node);

			break;
		case CodeType::CodeEnd:

			if (mStream->PeekToken(0).mType == TokenType::TokenEnd)
			{
				mStream->NextToken();
				return codebody;
			}
			ct = mStream->NextToken();
			if (ct.mType != TokenType::TokenEnd)
			{
				ct = mStream->NextToken();
			}
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

	return codebody;
	return nullptr;

}