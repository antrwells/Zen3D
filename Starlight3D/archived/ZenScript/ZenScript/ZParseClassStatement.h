#pragma once
#include "ZParseNode.h"


class ZParseClassStatement :
    public ZParseNode
{
public:

    ZParseClassStatement(ZTokenStream* stream);
    ZScriptNode* Parse();

private:

  
};

