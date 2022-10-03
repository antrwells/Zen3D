#pragma once
#include "ZParseNode.h"
class ZParseStatement :
    public ZParseNode
{
public:

    ZParseStatement(ZTokenStream* stream);
    ZScriptNode* Parse();

private:
};

