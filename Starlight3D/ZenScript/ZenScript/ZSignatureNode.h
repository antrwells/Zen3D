#pragma once
#include "ZScriptNode.h"
#include <vector> 

class ZSigParamNode;

class ZSignatureNode :
    public ZScriptNode
{
public:
    void AddParameter(ZSigParamNode* node);
    std::vector<ZSigParamNode*> GetParams() {
        return mParams;
    }
private:
    std::vector<ZSigParamNode*> mParams;
};

