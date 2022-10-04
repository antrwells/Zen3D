#pragma once
#include "ZParseNode.h"
class ZParseFor :
    public ZParseNode
{
public:

    ZParseFor(ZTokenStream* stream);
    ZScriptNode* Parse();

private:
};

