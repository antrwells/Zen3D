#pragma once
#include "ZScriptNode.h"
#include <vector>
#include <string>

class ZParametersNode;

class ZStatementNode :
    public ZScriptNode
{
public:

    void AddCallName(std::string name);
    void SetPars(ZParametersNode* pars);
    ZContextVar* Exec(std::initializer_list<ZContextVar*> args);

private:

    std::vector<std::string> mCallNames;
    ZParametersNode* mPars;

};

