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

ZContextVar* ZContextScope::FindVar(std::string name) {

	//if (mVarsMap.find(name) != mVarsMap.end()) {
//		return mVarsMap[name];
//	}
		//new_token.mType = token_map[old_token.mText];
	for (int i = 0; i < mVars.size(); i++) {
		if (mVars[i]->GetName() == name) {
			return mVars[i];
		}
	}
		assert(false);
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