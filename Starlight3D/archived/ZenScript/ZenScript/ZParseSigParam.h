#pragma once
#include "ZParseNode.h"
class ZParseSigParam :
    public ZParseNode
{
public:

    ZParseSigParam(ZTokenStream* stream);
    ZScriptNode* Parse();

private:
};

