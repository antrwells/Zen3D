#include "ZIfnode.h"
#include "ZExpressionNode.h"
#include "ZCodeBodyNode.h"
void ZIfNode::SetIfExpression(ZExpressionNode* expr)
{

	mIfExpr = expr;

}

void ZIfNode::SetTrueCode(ZCodeBodyNode* code) {

	mTrueCode = code;

}

void ZIfNode::SetElseCode(ZCodeBodyNode* code) {

	mElseCode = code;

}


ZContextVar* ZIfNode::Exec(const std::vector<ZContextVar*>& params)
{

	if (mIfExpr->Exec(std::vector<ZContextVar*>())->GetIntVal() >= 1)
	{
		mTrueCode->SetClassOwner(GetClassOwner());
		mTrueCode->Exec(params);
	}

	int a = 5;

	return nullptr;
}