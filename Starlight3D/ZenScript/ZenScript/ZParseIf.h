#pragma once
#include "ZParseNode.h"
class ZParseIf :
    public ZParseNode
{
public:

    ZParseIf(ZTokenStream* stream);
    ZScriptNode* Parse();

private:
};

