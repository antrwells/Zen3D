#include "ZVarsNode.h"
#include "ZContextVar.h"
#include "ZExpressionNode.h"
#include "ZCodeBodyNode.h"
#include "ZMethodNode.h"
#include "ZNewNode.h"
#include "ZScriptContext.h"
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

void ZVarsNode::AddVar(std::string name, ZNewNode* new_node) {

	VarPair* pair = new VarPair;
	pair->name = name;
	pair->new_node = new_node;
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
		ZClassNode* cls = nullptr;

		switch (mVarType) {
		case VarType::VarString:

			new_var->SetString(var->def->Exec({})->GetStringVal());

			break;
		case VarType::VarInt:
			
		{

			auto rv = var->def->Exec({});
			switch (rv->GetType()) {
			case VarType::VarFloat:
				new_var->SetInt((int)rv->GetFloatVal());
				break;
			case VarType::VarInt:
				new_var->SetInt(rv->GetIntVal());
			}
		}
			break;
		case VarType::VarFloat:

		{
		
			auto rv = var->def->Exec({});
			switch (rv->GetType()) {
			case VarType::VarFloat:
				new_var->SetFloat(rv->GetFloatVal());
				break;
			case VarType::VarInt:
				new_var->SetFloat(rv->GetIntVal());
			}
		}
			break;
		case VarType::VarInstance:


			cls = var->new_node->Exec(std::vector<ZContextVar*>())->GetClassVal();


			new_var->SetClass(cls);
			
			//auto owner = mCodeOwner->GetOwner();
			auto scope = ZScriptContext::CurrentContext->GetScope();//  owner->GetScope();
			scope->RegisterVar(new_var);
			break;
		}
		//if (mCodeOwner != nullptr) {
			int aa = 5;
			//auto owner = mCodeOwner->GetOwner();
			auto scope = ZScriptContext::CurrentContext->GetScope();
			scope->RegisterVar(new_var);

		
		

	}
	int b = 5;
	return nullptr;
}

void ZVarsNode::SetCodeOwner(ZCodeBodyNode* owner)
{
	mCodeOwner = owner;
}