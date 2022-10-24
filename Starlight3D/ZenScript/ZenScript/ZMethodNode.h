#pragma once
#include "ZScriptNode.h"
#include <string>
#include "VarTypes.h"
#include "ZContextScope.h"

class ZCodeBodyNode;
class ZSignatureNode;
class ZContextVar;
class ZExpressionNode;

class ZMethodNode : public ZScriptNode
{
public:

	void SetName(std::string name);
	void SetSignature(ZSignatureNode* node);
	void SetReturnType(VarType type);
	void SetCode(ZCodeBodyNode* code);
	std::string GetName();
	void SetScope(ZContextScope* scope);
	ZContextScope* GetScope();
	ZSignatureNode* GetSignature() {
		return mSignature;
	}
	ZMethodNode* Clone();
	//Exec
	ZContextVar* Exec(const std::vector<ZContextVar*>& params);
	void Bind();
	VarType GetReturnType() {
		return mReturnType;
	}
	void SetGuard(ZExpressionNode* g_e);
	static ZMethodNode* mCurrentNode;
	void SetVirtual(bool is_virtual);
	bool GetVirtual();
private:
	ZExpressionNode* mGuardExpr = nullptr;
	std::string mMethodName;
	ZSignatureNode* mSignature;
	VarType mReturnType;
	ZCodeBodyNode* mCode;
	ZContextScope* mScope;
	ZClassNode* mClass;
	bool mIsVirtual = false;
};

