#pragma once
#include "ZScriptNode.h"
#include <string>
class ZIncNode :
    public ZScriptNode
{
public:

    void SetVarName(std::string name);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);
    void SetNegative() {
        mNegative = true;
    }
private:

    std::string mVarName;
    size_t mNameHash;
    bool mNegative = false;

};

