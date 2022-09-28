#pragma once
#include "ZScriptNode.h"
#include <string>
#include "VarTypes.h"

class ZCodeBodyNode;
class ZSignatureNode;
class ZContextVar;

class ZMethodNode : public ZScriptNode
{
public:

	void SetName(std::string name);
	void SetSignature(ZSignatureNode* node);
	void SetReturnType(VarType type);
	void SetCode(ZCodeBodyNode* code);
	std::string GetName();

	//Exec
	ZContextVar* Exec(const std::vector<ZContextVar*>& params);

private:
	std::string mMethodName;
	ZSignatureNode* mSignature;
	VarType mReturnType;
	ZCodeBodyNode* mCode;

};

