#pragma once
#include "ZParseNode.h"
class ZParseParameters :
    public ZParseNode
{
public:

    ZParseParameters(ZTokenStream* stream);
    ZScriptNode* Parse();

private:
};

