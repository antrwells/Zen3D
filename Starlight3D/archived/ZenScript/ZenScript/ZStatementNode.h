#pragma once
#include "ZScriptNode.h"
#include <vector>
#include <string>

class ZCodeBodyNode;
class ZParametersNode;

class ZStatementNode :
    public ZScriptNode
{
public:

    void AddCallName(std::string name);
    void SetPars(ZParametersNode* pars);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);
    void SetCode(ZCodeBodyNode* code);

private:

    std::vector<std::string> mCallNames;
    ZParametersNode* mPars;
    ZCodeBodyNode* mCode;

};

