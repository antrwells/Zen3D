#pragma once
#include "ZScriptNode.h"

class ZExpressionNode;
class ZCodeBodyNode;

class ZWhileNode :
    public ZScriptNode
{
public:

    void SetExpression(ZExpressionNode* expr);
    void SetCode(ZCodeBodyNode* code);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);

private:

    ZExpressionNode* mExpr;
    ZCodeBodyNode* mCode;

};

