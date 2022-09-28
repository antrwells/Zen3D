#include "ZParseMethod.h"
#include "ZMethodNode.h"
#include "ZParseSignature.h"
#include "ZSignatureNode.h"
#include "ZParseCodeBody.h"
#include "ZCodeBodyNode.h"

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
	}

	int bb = 5;

	auto meth_name = mStream->NextToken();

	meth_node->SetName(meth_name.mText);

	auto parse_sig = new ZParseSignature(mStream);

	auto sig_node = (ZSignatureNode*)parse_sig->Parse();
	
	meth_node->SetSignature(sig_node);

	auto next_tok = mStream->AssertNextToken(TokenType::TokenEndOfLine);


	auto code_parse = new ZParseCodeBody(mStream);

	auto code_node = (ZCodeBodyNode*)code_parse->Parse();

	code_node->SetOwner(meth_node);

	meth_node->SetCode(code_node);

	int aa = 5;

	return meth_node;
}