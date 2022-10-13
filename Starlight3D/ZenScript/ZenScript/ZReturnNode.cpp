#include "ZReturnNode.h"
#include "ZExpressionNode.h"

void ZReturnNode::SetExpression(ZExpressionNode* expr)
{

	mExpr = expr;

}

ZContextVar* ZReturnNode::Exec(const std::vector<ZContextVar*>& params)
{
	ZExpressionNode::RecvType = mReturnType;
	return mExpr->Exec({});

}