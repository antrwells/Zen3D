#pragma once
#include "ZScriptNode.h"
#include <vector> 

class ZSigParamNode;

class ZSignatureNode :
    public ZScriptNode
{
public:
    void AddParameter(ZSigParamNode* node);

private:
    std::vector<ZSigParamNode*> mParams;
};

