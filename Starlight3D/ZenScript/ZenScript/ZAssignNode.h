#pragma once
#include "ZScriptNode.h"
#include <string>

class ZExpressionNode;

class ZAssignNode :
    public ZScriptNode
{
public:

    void SetVarName(std::string name);
    void SetValue(ZExpressionNode* val);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);
    

private:

    std::string mVarName;
    ZExpressionNode* mValue;

};

