#include "ZClassStatementNode.h"
#include "ZClassNode.h"
#include "ZScriptContext.h"
#include "ZContextScope.h"
#include "ZParametersNode.h"
#include "ZExpressionNode.h"
#include "ZMethodNode.h"
#include "ZSignatureNode.h"
#include "ZSigParamNode.h"
#include "ZMethodNode.h"
#include "ZClassNode.h"
ZContextVar* ZClassStatementNode::Exec(const std::vector<ZContextVar*>& params)
{
	//std::hash<std::string> hasher;
	//ZClassNode* top;



	int name_id = 0;

	while (true) {

		auto name = mNamesHash[name_id];
		ZClassNode* cls = nullptr;

		if (mCacheCls == nullptr) {
			if (ZScriptContext::CurrentContext->IsStaticClass(name)) {

				cls = ZScriptContext::CurrentContext->GetStaticClass(name);
				int bv = 1;
				mCacheCls = cls;

			}
			else {

				auto var = ZScriptContext::CurrentContext->GetScope()->FindVar(name);
				cls = var->GetClassVal();
				mCacheCls = cls;
			}
		}
		//auto cls = var->GetClassVal();
		ZMethodNode* meth = nullptr;
		if (mCacheMeth == nullptr) {
			meth = mCacheCls->GetMethod(mNamesHash[name_id + 1]);
			mCacheMeth = meth;
		}
		else {
			meth = mCacheMeth;
		}

		auto sig_node = meth->GetSignature();

		auto spars = sig_node->GetParams();

		auto pars = mPars->GetParameters();

		std::vector<ZContextVar*> vpars;

		for (int i = 0; i < pars.size(); i++) {

			auto par = pars[i];

			auto ttype = spars[i]->GetType();
			ZExpressionNode::RecvType = ttype;
			vpars.push_back(par->Exec(std::vector<ZContextVar*>()));


		}



		return mCacheCls->CallMethod(mNamesHash[name_id + 1],vpars);

		name_id++;
		break;
	}

	return nullptr;
}

void ZClassStatementNode::AddName(std::string name) {

	std::hash<std::string> hasher;
	mNames.push_back(name);
	mNamesHash.push_back(hasher(name));


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


	auto name = mNamesHash[0];

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
	return VarType::VarVoid;
}