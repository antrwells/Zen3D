#pragma once
#include "ZParseNode.h"
class ZParseValue :
    public ZParseNode
{
public:

    ZParseValue(ZTokenStream* stream);
    ZScriptNode* Parse();

private:

    std::string mName;
  

};

