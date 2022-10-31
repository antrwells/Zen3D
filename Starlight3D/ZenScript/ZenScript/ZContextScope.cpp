#include "ZContextScope.h"
#include <assert.h>
#include "ZClassNode.h"

ZContextScope::ZContextScope(std::string name) {

	mName = name;

}


void ZContextScope::RegisterVar(ZContextVar* var)
{
	
	mVars.push_back(var);
	//mVarsMap.insert(std::make_pair(var->GetName(),var));;

}

ZContextVar* ZContextScope::FindVar(size_t hash) {

	//if (mVarsMap.find(name) != mVarsMap.end()) {
//		return mVarsMap[name];
//	}
		//new_token.mType = token_map[old_token.mText];
	for (int i = (int)mVars.size()-1; i>=0; i--) {
		if (mVars[i]->GetHashName() == hash) {
			return mVars[i];
		}
	}
	
		return nullptr;

}

void ZContextScope::PushClass(ZClassNode* node) {

}

void ZContextScope::PopClass(ZClassNode* node) {


}

std::vector<ZContextVar*> ZContextScope::GetVars() {

	return mVars;

}

void ZContextScope::SetVars(std::vector<ZContextVar*> vars)
{

	mVars = vars;

}

ZContextScope* ZContextScope::Clone() {

	auto new_scope = new ZContextScope("class clone");
	
	new_scope->SetVars(mVars);
 
	return new_scope;

}

void ZContextScope::RemoveVar(ZContextVar* var)
{

	std::vector<ZContextVar*> new_vars;

	for (int i = 0; i < mVars.size(); i++) {

		if (mVars[i] == var)
		{

		}
		else {
			new_vars.push_back(mVars[i]);
		}

	}

	mVars = new_vars;

}