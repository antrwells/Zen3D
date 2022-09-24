#pragma once
#include "ZScriptNode.h"
#include <vector>
class ZCodeBodyNode :
    public ZScriptNode
{
public:

    ZCodeBodyNode();
    void AddNode(ZScriptNode* node);

private:
    std::vector<ZScriptNode*> mNodes;
};

