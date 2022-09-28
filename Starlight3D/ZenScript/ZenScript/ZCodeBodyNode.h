#pragma once
#include "ZScriptNode.h"
#include <vector>



class ZCodeBodyNode :
    public ZScriptNode
{
public:

    ZCodeBodyNode();
    void AddNode(ZScriptNode* node);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);

private:
    std::vector<ZScriptNode*> mNodes;
};

