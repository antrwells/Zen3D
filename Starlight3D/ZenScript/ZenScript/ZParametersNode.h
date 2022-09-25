#pragma once
#include "ZScriptNode.h"

class ZExpressionNode;

class ZParametersNode :
    public ZScriptNode
{
public:

    void AddExpression(ZExpressionNode* expr);

private:

    std::vector<ZExpressionNode*> mParNodes;

};

