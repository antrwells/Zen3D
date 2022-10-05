#include "ZWhileNode.h"
#include "ZExpressionNode.h"
#include "ZCodeBodyNode.h"

void ZWhileNode::SetCode(ZCodeBodyNode* code)
{

	mCode = code;

}

void ZWhileNode::SetExpression(ZExpressionNode* expr) {

	mExpr = expr;

}

ZContextVar* ZWhileNode::Exec(const std::vector<ZContextVar*>& params) {

	while (mExpr->Exec({})->GetIntVal() >= 1)
	{
		mCode->Exec({});
	}
	return nullptr;
}