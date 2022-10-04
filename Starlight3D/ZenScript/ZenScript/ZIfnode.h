#pragma once
#include "ZScriptNode.h"

class ZCodeBodyNode;
class ZExpressionNode;

class ZIfNode :
    public ZScriptNode
{
public:

    void SetIfExpression(ZExpressionNode* expr);
    void SetTrueCode(ZCodeBodyNode* code);
    void SetElseCode(ZCodeBodyNode* code);
    void SetElseIf(ZIfNode* code);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);

private:

    ZExpressionNode* mIfExpr = nullptr;
    ZCodeBodyNode* mTrueCode = nullptr;
    ZCodeBodyNode* mElseCode = nullptr;
    ZIfNode* mElseIf = nullptr;

};

