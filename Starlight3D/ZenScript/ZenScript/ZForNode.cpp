#include "ZForNode.h"
#include "ZAssignNode.h"
#include "ZExpressionNode.h"
#include "ZCodeBodyNode.h"

void ZForNode::SetInit(ZAssignNode* assign)
{

	mInitAssign = assign;

}

void ZForNode::SetUntil(ZExpressionNode* until)
{
	
	mUntilExpr = until;

}


void ZForNode::SetCode(ZCodeBodyNode* code) {

	mCode = code;

}

void ZForNode::SetIncAssign(ZAssignNode* assign)
{

	mIncAssign = assign;

}


ZContextVar* ZForNode::Exec(const std::vector<ZContextVar*>& params) {


	mInitAssign->Exec(params);
	while (mUntilExpr->Exec(params)->GetIntVal() == 1)
	{
		mCode->Exec(params);
		mIncAssign->Exec(params);
	}

	return nullptr;
}