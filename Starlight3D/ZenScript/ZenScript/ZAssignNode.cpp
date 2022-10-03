#include "ZAssignNode.h"
#include "ZExpressionNode.h"
#include "ZScriptContext.h"
#include "ZContextScope.h"
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

	}

	int aa = 5;
	return nullptr;
}