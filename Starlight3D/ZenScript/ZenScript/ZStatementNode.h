#pragma once
#include "ZScriptNode.h"
#include <vector>
#include <string>

class ZStatementNode :
    public ZScriptNode
{
public:

    void AddCallName(std::string name);

private:

    std::vector<std::string> mCallNames;

};

