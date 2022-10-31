#pragma once
#include "ZScriptNode.h"
#include <vector>
#include <string>
#include "ZSystemFunction.h"

class ZCodeBodyNode;
class ZParametersNode;
class ZSystemFunction;
class ZMethodNode;

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
    std::vector<size_t> mHashNames;
    ZParametersNode* mPars;
    ZCodeBodyNode* mCode;
    ZMethodNode* mCacheMeth = nullptr;
    ZSystemFunction mCacheFunc;

};

