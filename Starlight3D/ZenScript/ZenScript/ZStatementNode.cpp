#include "ZStatementNode.h"
#include "ZSystemFunctions.h"
#include "ZParametersNode.h"
#include "ZContextVar.h"
#include "ZExpressionNode.h"
#include "VarTypes.h"
#include "ZScriptContext.h"
#include "ZMethodNode.h"
#include "ZClassNode.h"
#include "ZSignatureNode.h"
#include "ZSigParamNode.h"

//#include "ZSystemFunction.h"
void ZStatementNode::AddCallName(std::string name) {

	mCallNames.push_back(name);

}

void ZStatementNode::SetPars(ZParametersNode* pars) {

	mPars = pars;

}


ZContextVar* ZStatementNode::Exec(const std::vector<ZContextVar*>& params)
{

	//std::initializer_list<ZContextVar*> args;


	auto top_class = ZScriptContext::CurrentContext->GetClass();//  GetClassOwner();

	//auto meth = top_class->FindMethod(this->mCallNames[0]);

	ZMethodNode* meth_node = nullptr;

	if (mCallNames.size() != mHashNames.size()) {
		std::hash<std::string> hasher;
		for (int i = 0; i < mCallNames.size();i++)
		{
			mHashNames.push_back(hasher(mCallNames[i]));
		}
	}

	if (mCacheMeth == nullptr) {
		if (top_class != nullptr) {
			if (top_class->FindMethod(mHashNames[0]) != nullptr)
			{

				//return 
				meth_node = top_class->GetMethod(mHashNames[0]);
				mCacheMeth = meth_node;
			}
		}
	}

	if (meth_node != nullptr) {

		auto sig = meth_node->GetSignature();

		auto pars = mPars->GetParameters();

		auto spars = sig->GetParams();

		std::vector<ZContextVar*> vpars;

		for (int i = 0; i < pars.size(); i++) {

			auto par = pars[i];

			auto type = spars[i]->GetType();

			vpars.push_back(par->Exec(std::vector<ZContextVar*>()));


		}

		return mCacheMeth->Exec(vpars); //; // top_class->CallMethod(mHashNames[0], vpars);

	}
	else {

//		auto sig = meth_node->GetSignature();

		auto pars = mPars->GetParameters();

	//	auto spars = sig->GetParams();

		std::vector<ZContextVar*> vpars;

		for (int i = 0; i < pars.size(); i++) {

			auto par = pars[i];

		//	auto type = spars[i]->GetType();

			vpars.push_back(par->Exec(std::vector<ZContextVar*>()));


		}



		//args = { 25,40,30 };

		ZContextVar* v1 = new ZContextVar("Test", VarType::VarInt,"int",false);
		v1->SetInt(2555);



		//if (meth != nullptr) {

	//		meth->Exec();

	//	}

		if (mCacheFunc.IsAlive()==false) {
			mCacheFunc = ZSystemFunctions::sFuncs->GetFunction(mHashNames[0]);
		
		}
		else {
			int aa = 5;
		}

		return mCacheFunc.CallFunction(vpars);
		//return ZSystemFunctions::sFuncs->CallFunction(mHashNames[0], vpars);


	}


	return nullptr;
}

void ZStatementNode::SetCode(ZCodeBodyNode* code)
{

	mCode = code;

}