#pragma once
#include "ZScriptNode.h"
#include <vector>
#include <string>
#include "DataTypes.h"

class ZVarsNode :
    public ZScriptNode
{
public:

    void SetType(VarType type);
    void AddVar(std::string name);

private:

    VarType mVarType;
    std::vector<std::string> mVarNames;

};

