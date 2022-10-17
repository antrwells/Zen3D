#include "ZParseIf.h"
#include <assert.h>
#include "ZExpressionNode.h"
#include "ZParseExpression.h"
#include "ZIfnode.h"
#include "ZParseCodeBody.h"

ZParseIf::ZParseIf(ZTokenStream* stream) : ZParseNode(stream) {

}

ZScriptNode* ZParseIf::Parse() {
	
	auto tok = mStream->NextToken();

	if(tok.mType != TokenType::TokenIf && tok.mType != TokenType::TokenElseIf)
	{
		mStream->Back();
		if (mStream->PeekToken(0).mType != TokenType::TokenIf) {
		//	assert(0);
		}
		mStream->NextToken();
		auto b = mStream->NextToken();
		int bbb = 5;
	}

	auto parse_e = new ZParseExpression(mStream);
	auto e_node = (ZExpressionNode*)parse_e->Parse();
	int a = 5;

	auto if_node = new ZIfNode;
	if_node->SetIfExpression(e_node);

	auto parse_code = new ZParseCodeBody(mStream);
	auto true_code = (ZCodeBodyNode*)parse_code->Parse();

	if_node->SetTrueCode(true_code);

	//auto cn = mStream->NextToken();

	mStream->Back();
	auto ct = mStream->NextToken();
	
	if (ct.mType == TokenEndOfLine) {
		ct = mStream->NextToken();
	}

	if (ct.mType == TokenType::TokenElseIf)
	{
		mStream->Back();
		auto eif_parse = new ZParseIf(mStream);
		auto eif_node = (ZIfNode*)eif_parse->Parse();
		if_node->SetElseIf(eif_node);
	
	}
	if (ct.mType == TokenType::TokenElse)
	{

		int aa = 5;
		auto par_code = new ZParseCodeBody(mStream);
		auto code_node = (ZCodeBodyNode*)par_code->Parse();
		if_node->SetElseCode(code_node);

	}

	if (mStream->PeekToken(0).mType == TokenType::TokenEnd)
	{
		mStream->NextToken();
	}

	return if_node;

	return nullptr;
}