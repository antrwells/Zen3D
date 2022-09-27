#include "ZStatementNode.h"


void ZStatementNode::AddCallName(std::string name) {

	mCallNames.push_back(name);

}

void ZStatementNode::SetPars(ZParametersNode* pars) {

	mPars = pars;

}

ZContextVar* ZStatementNode::Exec(std::initializer_list<ZContextVar*> args)
{



	return nullptr;
}