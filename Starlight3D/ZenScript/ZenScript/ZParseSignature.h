#pragma once
#include "ZParseNode.h"
class ZParseSignature :
    public ZParseNode
{
public:

    ZParseSignature(ZTokenStream* stream);
    ZScriptNode* Parse();

private:
};

