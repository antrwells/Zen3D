#pragma once
#include "ZScriptNode.h"
#include <vector>



class ZCodeBodyNode :
    public ZScriptNode
{
public:

    ZCodeBodyNode();
    void AddNode(ZScriptNode* node);
    ZContextVar* Exec(std::initializer_list<ZContextVar*> args);

private:
    std::vector<ZScriptNode*> mNodes;
};

