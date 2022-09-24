#pragma once
#include "ZParseNode.h"
#include "ZTokenStream.h"
class ZParseCodeBody :
    public ZParseNode
{
public:

    ZParseCodeBody(ZTokenStream* stream);
    ZScriptNode* Parse();

private:
};

