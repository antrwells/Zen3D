#pragma once
#include "ZScriptNode.h"
#include <vector>
#include <string>
#include "VarTypes.h"

class ZExpressionNode;

struct VarPair {
    std::string name;
    ZExpressionNode* def = nullptr;
};


class ZCodeBodyNode;
class ZClassNode;

class ZVarsNode :
    public ZScriptNode
{
public:

    void SetType(VarType type);
    void AddVar(std::string name, ZExpressionNode* def);
    std::vector<VarPair*> GetVars();
    VarType GetType();
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);
    void SetCodeOwner(ZCodeBodyNode* code);

private:

    VarType mVarType;
    //std::vector<std::string> mVarNames;
    std::vector<VarPair*> mVars;
    ZClassNode* mClassOwner = nullptr;
    ZCodeBodyNode* mCodeOwner = nullptr;


};

