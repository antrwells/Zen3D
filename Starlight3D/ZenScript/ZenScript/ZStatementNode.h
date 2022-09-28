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
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);

private:

    std::vector<std::string> mCallNames;
    ZParametersNode* mPars;

};
