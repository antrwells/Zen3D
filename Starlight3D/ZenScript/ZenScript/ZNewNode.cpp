#include "ZNewNode.h"
#include "ZParametersNode.h"
#include "ZScriptContext.h"
#include "VarTypes.h"
#include "ZClassNode.h"
#include <assert.h>
#include "ZExpressionNode.h"
void ZNewNode::SetClassName(std::string name) {

	mClassName = name;

}

std::string ZNewNode::GetClassName() {

	return mClassName;

}

void ZNewNode::SetParameters(ZParametersNode* parameters)
{

	mPars = parameters;

}

ZParametersNode* ZNewNode::GetParameters() {

	return mPars;

}

ZContextVar* ZNewNode::Exec(const std::vector<ZContextVar*>& params)
{

	

	

	int a = 5;

	std::vector<ZContextVar*> pars;

	auto epars = mPars->GetParameters();

	for (int i = 0; i < epars.size(); i++) {

		auto val = epars[i]->Exec(std::vector<ZContextVar*>());
		pars.push_back(val);

	}
	auto new_cls = ZScriptContext::CurrentContext->CreateInstance(mClassName, "", pars);
	if (new_cls == nullptr)
	{
		assert(false);
	}
	//new_cls->CallMethod(mClassName, pars);

	ZContextVar* result = new ZContextVar("a", VarType::VarInstance,new_cls->GetBaseName(),false);
	result->SetClass(new_cls);

	return result;
}