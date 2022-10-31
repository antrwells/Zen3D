#include "ZIncNode.h"
#include "ZScriptContext.h"
#include "ZContextScope.h"
#include "ZContextVar.h"
#include "VarTypes.h"

void ZIncNode::SetVarName(std::string name) {

	std::hash<std::string> hasher;
	mVarName = name;
	mNameHash = hasher(name);

}

ZContextVar* ZIncNode::Exec(const std::vector<ZContextVar*>& params)
{

	auto evar = ZScriptContext::CurrentContext->GetScope()->FindVar(mNameHash);

	int val = 1;
	if (mNegative) {
		val = -1;
	}

	switch (evar->GetType()) {
	case VarType::VarInt:
		evar->SetInt(evar->GetIntVal() + val);
		break;
	case VarType::VarFloat:
		evar->SetFloat(evar->GetFloatVal() + val);
		break;

	}
	return nullptr;


}