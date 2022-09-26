#pragma once
#include "ZParseNode.h"
class ZParseVars :
    public ZParseNode
{
public:

    ZParseVars(ZTokenStream* stream);
    ZScriptNode* Parse();

private:

};

