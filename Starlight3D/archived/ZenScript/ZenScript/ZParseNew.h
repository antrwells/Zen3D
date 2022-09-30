#pragma once
#include "ZParseNode.h"
class ZParseNew :
    public ZParseNode
{
public:

    ZParseNew(ZTokenStream* stream);
    ZScriptNode* Parse();

private:
};

