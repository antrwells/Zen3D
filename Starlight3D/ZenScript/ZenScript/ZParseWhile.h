#pragma once
#include "ZParseNode.h"
class ZParseWhile :
    public ZParseNode
{
public:

    ZParseWhile(ZTokenStream* stream);
    ZScriptNode* Parse();

private:
};

