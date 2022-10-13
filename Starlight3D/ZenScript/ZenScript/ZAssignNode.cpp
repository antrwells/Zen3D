#include "ZAssignNode.h"
#include "ZExpressionNode.h"
#include "ZScriptContext.h"
#include "ZContextScope.h"
#include "ZClassNode.h"
void ZAssignNode::SetVarName(std::string name)
{

	mVarName = name;

}

void ZAssignNode::SetValue(ZExpressionNode* node)
{

	mValue = node;

}

ZContextVar* ZAssignNode::Exec(const std::vector<ZContextVar*>& params)
{

	ZContextVar* evar = nullptr;
	if (ZScriptContext::CurrentContext->IsStaticClass(mVarName))
	{
		auto scls = ZScriptContext::CurrentContext->GetStaticClass(mVarName);
		evar = scls->FindVar(mMember);
	}
	else {
		evar = ZScriptContext::CurrentContext->GetScope()->FindVar(mVarName);

		if (mMember != "")
		{
			evar = evar->GetClassVal()->FindVar(mMember);
			int aa = 5;

		}
	}

	switch (evar->GetType()) {
	case VarType::VarInt:
		
		ZExpressionNode::RecvType = VarInt;
		evar->SetInt(mValue->Exec(std::vector<ZContextVar*>())->GetIntVal());
		
		break;
	case VarType::VarFloat:
		ZExpressionNode::RecvType = VarFloat;
		evar->SetFloat(mValue->Exec(std::vector<ZContextVar*>())->GetFloatVal());
		break;
	case VarType::VarString:
		ZExpressionNode::RecvType = VarString;
		evar->SetString(mValue->Exec(std::vector<ZContextVar*>())->GetStringVal());

		break;
	case VarType::VarCObj:
		ZExpressionNode::RecvType = VarCObj;
		evar->SetCObj(mValue->Exec({})->GetCObj());
		break;
	case VarType::VarInstance:
		ZExpressionNode::RecvType = VarInstance;
		evar->SetClass(mValue->Exec({})->GetClassVal());
		int a = 2;

	}

	int aa = 5;
	return nullptr;
}

void ZAssignNode::SetMemberName(std::string name) {

	mMember = name;

}