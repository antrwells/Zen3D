#pragma once
#include "ZScriptNode.h"
#include <vector>
#include <string>
#include "VarTypes.h"

class ZVarsNode :
    public ZScriptNode
{
public:

    void SetType(VarType type);
    void AddVar(std::string name);
    std::vector<std::string> GetNames();
    VarType GetType();

private:

    VarType mVarType;
    std::vector<std::string> mVarNames;

};

