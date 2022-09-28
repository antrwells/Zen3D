#include "ZStatementNode.h"
#include "ZSystemFunctions.h"
#include "ZParametersNode.h"
#include "ZContextVar.h"
#include "ZExpressionNode.h"
#include "VarTypes.h"

void ZStatementNode::AddCallName(std::string name) {

	mCallNames.push_back(name);

}

void ZStatementNode::SetPars(ZParametersNode* pars) {

	mPars = pars;

}

ZContextVar* ZStatementNode::Exec(const std::vector<ZContextVar*>& params)
{

	//std::initializer_list<ZContextVar*> args;




	auto pars = mPars->GetParameters();

	std::vector<ZContextVar*> vpars;

	for (int i = 0; i < pars.size(); i++) {

		auto par = pars[i];
		vpars.push_back(par->Exec(std::vector<ZContextVar*>() ));


	}

	

	//args = { 25,40,30 };

	ZContextVar* v1 = new ZContextVar("Test", VarType::VarInt);
	v1->SetInt(2555);

	ZSystemFunctions::sFuncs->CallFunction(mCallNames[0],vpars);


	return nullptr;
}