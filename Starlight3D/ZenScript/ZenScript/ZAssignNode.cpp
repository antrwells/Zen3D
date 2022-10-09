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

	auto evar = ZScriptContext::CurrentContext->GetScope()->FindVar(mVarName);

	if (mMember != "")
	{
		evar = evar->GetClassVal()->FindVar(mMember);
		int aa = 5;

	}

	switch (evar->GetType()) {
	case VarType::VarInt:
		
		evar->SetInt(mValue->Exec(std::vector<ZContextVar*>())->GetIntVal());
		
		break;
	case VarType::VarFloat:
		evar->SetFloat(mValue->Exec(std::vector<ZContextVar*>())->GetFloatVal());
		break;
	case VarType::VarString:
		evar->SetString(mValue->Exec(std::vector<ZContextVar*>())->GetStringVal());

		break;
	case VarType::VarCObj:
		evar->SetCObj(mValue->Exec({})->GetCObj());
		break;
	case VarType::VarInstance:
		evar->SetClass(mValue->Exec({})->GetClassVal());
		int a = 2;

	}

	int aa = 5;
	return nullptr;
}

void ZAssignNode::SetMemberName(std::string name) {

	mMember = name;

}