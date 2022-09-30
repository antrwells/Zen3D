#pragma once
#include "ZScriptNode.h"

#include <string>
#include <vector>

class ZParametersNode;

class ZClassStatementNode :
    public ZScriptNode
{
public:
    void AddName(std::string name);
    std::vector<std::string> GetNames();
    ZParametersNode* GetParameters();
    void SetParameters(ZParametersNode* parameters);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);
private:

    std::vector<std::string> mNames;
    ZParametersNode* mPars;


};

