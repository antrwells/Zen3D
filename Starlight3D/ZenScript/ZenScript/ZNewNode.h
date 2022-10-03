#pragma once
#include "ZScriptNode.h"
#include <string>

class ZParametersNode;

class ZNewNode :
    public ZScriptNode
{
public:

    void SetClassName(std::string name);
    std::string GetClassName();
    void SetParameters(ZParametersNode* parameters);
    ZParametersNode* GetParameters();
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);

private:
    
    std::string mClassName;
    ZParametersNode* mPars;

};

