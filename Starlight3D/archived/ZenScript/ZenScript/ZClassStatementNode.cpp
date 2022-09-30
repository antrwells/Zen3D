#include "ZClassStatementNode.h"
#include "ZClassNode.h"
#include "ZScriptContext.h"
#include "ZContextScope.h"

ZContextVar* ZClassStatementNode::Exec(const std::vector<ZContextVar*>& params)
{

	ZClassNode* top;

	int name_id = 0;

	while (true) {

		auto name = mNames[name_id];

		auto var = ZScriptContext::CurrentScope->FindVar(name);


		name_id++;
	}

	return nullptr;
}

void ZClassStatementNode::AddName(std::string name) {

	mNames.push_back(name);

}

void ZClassStatementNode::SetParameters(ZParametersNode* parameters)
{

	mPars = parameters;

}

ZParametersNode* ZClassStatementNode::GetParameters() {

	return mPars;

}

std::vector<std::string> ZClassStatementNode::GetNames() {

	return mNames;

}
