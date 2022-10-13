#include "ZClassStatementNode.h"
#include "ZClassNode.h"
#include "ZScriptContext.h"
#include "ZContextScope.h"
#include "ZParametersNode.h"
#include "ZExpressionNode.h"
#include "ZMethodNode.h"
#include "ZSignatureNode.h"
#include "ZSigParamNode.h"
ZContextVar* ZClassStatementNode::Exec(const std::vector<ZContextVar*>& params)
{

	ZClassNode* top;

	int name_id = 0;

	while (true) {

		auto name = mNames[name_id];
		ZClassNode* cls = nullptr;

		if (ZScriptContext::CurrentContext->IsStaticClass(name)) {

			cls = ZScriptContext::CurrentContext->GetStaticClass(name);
			int bv = 1;


		}
		else {

			auto var = ZScriptContext::CurrentContext->GetScope()->FindVar(name);
			cls = var->GetClassVal();
		}
		//auto cls = var->GetClassVal();

		auto meth_node = cls->GetMethod(mNames[name_id + 1]);

		auto sig_node = meth_node->GetSignature();

		auto spars = sig_node->GetParams();

		auto pars = mPars->GetParameters();

		std::vector<ZContextVar*> vpars;

		for (int i = 0; i < pars.size(); i++) {

			auto par = pars[i];

			auto ttype = spars[i]->GetType();
			ZExpressionNode::RecvType = ttype;
			vpars.push_back(par->Exec(std::vector<ZContextVar*>()));


		}



		return cls->CallMethod(mNames[name_id + 1],vpars);

		name_id++;
		break;
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


VarType ZClassStatementNode::GetReturnType() {


	auto name = mNames[0];

	auto var = ZScriptContext::CurrentContext->GetScope()->FindVar(name);

	auto cls = var->GetClassVal();

	auto meths = cls->GetMethods();

	auto m_name = mNames[1];

	for (int i = 0; i < meths.size(); i++)
	{
	
		auto meth = meths[i];
		if (meth->GetName() == m_name) {
			return meth->GetReturnType();
		}

	}

}