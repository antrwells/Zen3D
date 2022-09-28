#pragma once
#include "ZScriptNode.h"

class ZExpressionNode;

class ZParametersNode :
    public ZScriptNode
{
public:

    void AddExpression(ZExpressionNode* expr);
    ZExpressionNode* GetParameter(int index);
    std::vector<ZExpressionNode*> GetParameters();

private:

    std::vector<ZExpressionNode*> mParNodes;

};

