#pragma once
#include "ZParseNode.h"
class ZParseReturn :
    public ZParseNode
{
public:

    ZParseReturn(ZTokenStream* stream);
    ZScriptNode* Parse();

private:
};

