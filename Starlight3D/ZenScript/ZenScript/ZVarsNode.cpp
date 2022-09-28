#include "ZVarsNode.h"
#include "ZContextVar.h"
#include "ZExpressionNode.h"
#include "ZCodeBodyNode.h"
#include "ZMethodNode.h"
void ZVarsNode::SetType(VarType type) {

	mVarType = type;

}

void ZVarsNode::AddVar(std::string name,ZExpressionNode* def) {

	VarPair* pair = new VarPair;
	pair->name = name;
	pair->def = def;
	//mVarNames.push_back(name);
	mVars.push_back(pair);

}

std::vector<VarPair*> ZVarsNode::GetVars() {

	return mVars;

}

VarType ZVarsNode::GetType() {

	return mVarType;

}

ZContextVar* ZVarsNode::Exec(const std::vector<ZContextVar*>& params)
{

	for (int i = 0; i < mVars.size(); i++)
	{

		auto var = mVars[i];
		ZContextVar* new_var = new ZContextVar(var->name, this->mVarType);

		switch (mVarType) {
		case VarType::VarInt:
			
			new_var->SetInt(var->def->Exec(std::vector<ZContextVar*>())->GetIntVal());
			
			break;
		case VarType::VarFloat:

			new_var->SetFloat(var->def->Exec(std::vector<ZContextVar*>())->GetFloatVal());

			break;
		}
		if (mCodeOwner != nullptr) {
			int aa = 5;
			auto owner = mCodeOwner->GetOwner();
			auto scope = owner->GetScope();
			scope->RegisterVar(new_var);

		}
		

	}
	int b = 5;
	return nullptr;
}

void ZVarsNode::SetCodeOwner(ZCodeBodyNode* owner)
{
	mCodeOwner = owner;
}