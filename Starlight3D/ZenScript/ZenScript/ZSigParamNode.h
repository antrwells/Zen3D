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
    std::string GetName() {
        return mParName;
    }
    VarType GetType() {
        return mParType;
    }

private:
    std::string mParName;
    VarType mParType;
};

