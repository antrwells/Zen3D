#pragma once
#include "ZParseNode.h"
class ZParseAssign :
    public ZParseNode
{
public:

    ZParseAssign(ZTokenStream* stream);
    ZScriptNode* Parse();

private:
};

