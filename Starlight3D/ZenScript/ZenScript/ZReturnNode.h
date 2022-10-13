#pragma once
#include "ZScriptNode.h"
#include "VarTypes.h"
class ZExpressionNode;

class ZReturnNode :
    public ZScriptNode
{
public:

    void SetExpression(ZExpressionNode* expr);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);
    void SetReturnType(VarType type) {
        mReturnType = type;
    }
private:

    VarType mReturnType = VarType::VarVoid;
    ZExpressionNode* mExpr;

};

