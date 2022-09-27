#pragma once
#include "ZScriptNode.h"
#include <string>
#include "VarTypes.h"
class ZSigParamNode :
    public ZScriptNode
{
public:

    void SetName(std::string);
    void SetType(VarType type);

private:
    std::string mParName;
    VarType mParType;
};

