#include "ZParseAssign.h"
#include "ZAssignNode.h"
#include "ZParseExpression.h"

ZParseAssign::ZParseAssign(ZTokenStream* stream) : ZParseNode(stream)
{



}


ZScriptNode* ZParseAssign::Parse() {

	auto tok = mStream->NextToken();

	if (tok.mType == TokenType::TokenPeriod)
	{
		mStream->Back();
		mStream->Back();
		tok = mStream->NextToken();
	}

	auto r_node = new ZAssignNode;

	r_node->SetVarName(tok.mText);

	auto tt = mStream->NextToken();

	if (tt.mType == TokenType::TokenPeriod)
	{
		auto en = mStream->NextToken();
		r_node->SetMemberName(en.mText);
	}
	else {
		mStream->Back();
	}

	int bb = 5;

	mStream->NextToken();

	auto p_val = new ZParseExpression(mStream);
	auto val_node = p_val->Parse();

	r_node->SetValue((ZExpressionNode*)val_node);


	int aa = 5;

	return r_node;

}