#include "ZParseMethod.h"
#include "ZMethodNode.h"
#include "ZParseSignature.h"
#include "ZSignatureNode.h"
#include "ZParseCodeBody.h"
#include "ZCodeBodyNode.h"
#include "ZParseExpression.h"
#include "ZExpressionNode.h"

ZParseMethod::ZParseMethod(ZTokenStream* stream) : ZParseNode(stream)
{

}

ZScriptNode* ZParseMethod::Parse()
{
	
	auto meth_node = new ZMethodNode;

	auto return_type = mStream->NextToken();

	switch (return_type.mType) {
	case TokenType::TokenInt:
		meth_node->SetReturnType(VarType::VarInt);
		break;
	case TokenType::TokenFloat:
		meth_node->SetReturnType(VarType::VarFloat);
		break;
	case TokenType::TokenString:
		meth_node->SetReturnType(VarType::VarString);
		break;
	case TokenType::TokenVoid:
		meth_node->SetReturnType(VarType::VarVoid);
	}

	int bb = 5;

	auto meth_name = mStream->NextToken();

	if (meth_name.mType == TokenType::TokenLeftPara)
	{

		mStream->Back();
		mStream->Back();
		meth_name = mStream->NextToken();

	}

	meth_node->SetName(meth_name.mText);

	auto parse_sig = new ZParseSignature(mStream);

	auto sig_node = (ZSignatureNode*)parse_sig->Parse();
	
	meth_node->SetSignature(sig_node);

	mStream->Back();

	auto next_tok = mStream->NextToken();

	if (next_tok.mType == TokenType::TokenRightPara)
	{

		//next_tok=mStream->NextToken();

	}

	auto nn = mStream->PeekToken(0);

	if (nn.mType == TokenType::TokenColon)
	{

		mStream->NextToken();
		auto g_ex = new ZParseExpression(mStream);
		auto g_n = (ZExpressionNode*)g_ex->Parse();

		meth_node->SetGuard(g_n);
	}

//	next_tok = mStream->NextToken();
	//next_tok = mStream->NextToken();


	auto code_parse = new ZParseCodeBody(mStream);

	auto code_node = (ZCodeBodyNode*)code_parse->Parse();

	code_node->SetOwner(meth_node);

	meth_node->SetCode(code_node);

	

	int aa = 5;

	return meth_node;
}