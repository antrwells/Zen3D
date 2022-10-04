#pragma once
#include "ZScriptNode.h"

class ZExpressionNode;

class ZReturnNode :
    public ZScriptNode
{
public:

    void SetExpression(ZExpressionNode* expr);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);

private:

    ZExpressionNode* mExpr;

};

