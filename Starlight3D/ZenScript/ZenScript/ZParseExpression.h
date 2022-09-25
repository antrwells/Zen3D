#pragma once
#include "ZParseNode.h"
class ZParseExpression :
    public ZParseNode
{
public:

    ZParseExpression(ZTokenStream* stream);
    ZScriptNode* Parse();

private:

};

