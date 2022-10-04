#pragma once
#include "ZScriptNode.h"

class ZAssignNode;
class ZExpressionNode;
class ZCodeBodyNode;

class ZForNode :
    public ZScriptNode
{
public:

    void SetInit(ZAssignNode* assign);
    void SetUntil(ZExpressionNode* until);
    void SetIncAssign(ZAssignNode* inc);
    void SetCode(ZCodeBodyNode* code);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);

private:

    ZAssignNode* mInitAssign;
    ZExpressionNode* mUntilExpr;
    ZAssignNode* mIncAssign;
    ZCodeBodyNode* mCode;

};

