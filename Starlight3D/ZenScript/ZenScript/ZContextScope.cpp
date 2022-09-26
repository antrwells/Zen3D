#include "ZContextScope.h"
#include <assert.h>
#include "ZClassNode.h"

ZContextScope::ZContextScope(std::string name) {

	mName = name;

}


void ZContextScope::RegisterVar(ZContextVar* var)
{
	
	mVars.push_back(var);
	mVarsMap.insert(std::make_pair(var->GetName(),var));;

}

ZContextVar* ZContextScope::FindVar(std::string name) {

	if (mVarsMap.find(name) != mVarsMap.end()) {
		return mVarsMap[name];
	}
		//new_token.mType = token_map[old_token.mText];
		assert(false);
		return nullptr;

}

void ZContextScope::PushClass(ZClassNode* node) {

}

void ZContextScope::PopClass(ZClassNode* node) {


}