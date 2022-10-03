#include "ZParametersNode.h"

void ZParametersNode::AddExpression(ZExpressionNode* expr) {

	mParNodes.push_back(expr);

}

ZExpressionNode* ZParametersNode::GetParameter(int index) {

	return mParNodes[index];

}

std::vector<ZExpressionNode*> ZParametersNode::GetParameters() {

	return mParNodes;

}