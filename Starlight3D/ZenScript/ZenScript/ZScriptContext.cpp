#include "ZScriptContext.h"
#include "ZContextScope.h"
#include <string>
#include "ZClassNode.h"
#include <assert.h>
#include "ZSystemFunctions.h"
#include "ZSystemFunction.h"
#include "VarTypes.h"

ZContextVar* sysfunc_printf(const std::vector<ZContextVar*>& args)
{

	for (auto&& arg : args)
	{
		switch (arg->GetType())
		{
		case VarType::VarInt:
			printf("%d\n",arg->GetIntVal());
			break;
		case VarType::VarFloat:
			printf("%f\n", arg->GetFloatVal());
			break;
		case VarType::VarString:
			printf(arg->GetStringVal().c_str());
			printf("\n");
			break;
		default:
			assert(false);
			break;
		}
	}
	return nullptr;
}

ZScriptContext::ZScriptContext() {

	SetupSystem();

}

void ZScriptContext::SetupSystem() {

	ZSystemFunction printf("printf", sysfunc_printf);

	ZSystemFunctions* tmp = new ZSystemFunctions;
	tmp->RegisterFunction(printf);


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