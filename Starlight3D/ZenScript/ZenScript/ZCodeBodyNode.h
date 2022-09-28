#pragma once
#include "ZScriptNode.h"
#include <vector>

class ZMethodNode;

class ZCodeBodyNode :
    public ZScriptNode
{
public:

    ZCodeBodyNode();
    void AddNode(ZScriptNode* node);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);
    void SetOwner(ZMethodNode* node);
    ZMethodNode* GetOwner();


private:
    std::vector<ZScriptNode*> mNodes;
    ZMethodNode* mOwner;
};

