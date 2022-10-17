#pragma once
#include "ZScriptNode.h"
#include <vector>
#include <string>
#include "VarTypes.h"

class ZExpressionNode;
class ZNewNode;

struct VarPair {
    std::string name;
    ZExpressionNode* def = nullptr;
    ZNewNode* new_node = nullptr;
};



class ZCodeBodyNode;
class ZClassNode;
//class ZNewNode;


class ZVarsNode :
    public ZScriptNode
{
public:

    void SetType(VarType type);
    void AddVar(std::string name, ZExpressionNode* def);
    void AddVar(std::string name, ZNewNode* new_node);
    void AddVar(std::string name);
    std::vector<VarPair*> GetVars();
    VarType GetType();
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);
    void SetCodeOwner(ZCodeBodyNode* code);
    void SetBaseType(std::string);
    std::string GetBaseType();

    
private:

    VarType mVarType;
    //std::vector<std::string> mVarNames;
    std::vector<VarPair*> mVars;
    ZClassNode* mClassOwner = nullptr;
    ZCodeBodyNode* mCodeOwner = nullptr;
    std::string TypeID;


};

