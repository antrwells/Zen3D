#include "ZScriptContext.h"
#include "ZContextScope.h"
#include <string>
#include "ZClassNode.h"
#include <assert.h>

ZScriptContext::ZScriptContext() {

	
}


void ZScriptContext::AddNode(ZMainNode* node) {

	auto classes = node->GetClasses();

	for (int i = 0; i < classes.size(); i++) {

		mClasses.push_back(classes[i]);

	}

}

ZClassNode* ZScriptContext::CreateInstance(std::string name,std::string instance_name) {

	for (int i = 0; i < mClasses.size(); i++) {

		auto cls = mClasses[i];

		if (cls->GetName() == name) {

			return cls->CreateInstance(instance_name);

		}

	}
	//*ERROR*
	assert(false);
	return nullptr;

}