#pragma once
#include "ZScriptNode.h"
#include <string>
#include "DataTypes.h"

class ZCodeBodyNode;
class ZSignatureNode;

class ZMethodNode : public ZScriptNode
{
public:

	void SetName(std::string name);
	void SetSignature(ZSignatureNode* node);
	void SetReturnType(VarType type);
	void SetCode(ZCodeBodyNode* code);

private:
	std::string mMethodName;
	ZSignatureNode* mSignature;
	VarType mReturnType;
	ZCodeBodyNode* mCode;

};

