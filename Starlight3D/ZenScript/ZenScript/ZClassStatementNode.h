#pragma once
#include "ZScriptNode.h"

#include <string>
#include <vector>
#include "VarTypes.h"

class ZParametersNode;
class ZMethodNode;
class ZClassNode;



class ZClassStatementNode :
    public ZScriptNode
{
public:
    void AddName(std::string name);
    std::vector<std::string> GetNames();
    std::vector<size_t> GetHashNames();
    ZParametersNode* GetParameters();
    void SetParameters(ZParametersNode* parameters);
    ZContextVar* Exec(const std::vector<ZContextVar*>& params);
    VarType GetReturnType();
private:

    std::vector<std::string> mNames;
    std::vector<size_t> mNamesHash;
    ZParametersNode* mPars;
    ZClassNode* mCacheCls = nullptr;
    ZMethodNode* mCacheMeth = nullptr;


};

