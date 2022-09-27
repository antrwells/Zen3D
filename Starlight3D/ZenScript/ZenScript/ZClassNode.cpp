#include "ZClassNode.h"
#include "ZMethodNode.h"
#include "ZContextScope.h"
#include "ZVarsNode.h"
#include <assert.h>

void ZClassNode::SetName(std::string name) {

	mClassName = name;

}

void ZClassNode::AddMethod(ZMethodNode* node)
{

	mMethods.push_back(node);

}

void ZClassNode::SetMethods(std::vector<ZMethodNode*> methods) {

	mMethods = methods;

}

std::vector<ZMethodNode*> ZClassNode::GetMethods() {

	return mMethods;

}

std::string ZClassNode::GetName() {

	return mClassName;

}

void ZClassNode::AddVars(ZVarsNode* node)
{

	mVars.push_back(node);

}

void ZClassNode::CreateScope() {

	mInstanceScope = new ZContextScope("Class:" + mClassName + " Instance");

}

void ZClassNode::PopulateScope() {

	for (int i = 0; i < mVars.size(); i++) {

		auto vars = mVars[i];

		auto names = vars->GetNames();
		auto type = vars->GetType();

		for (int j = 0; j < names.size(); j++)
		{

			ZContextVar* new_var = new ZContextVar(names[j], type);

			mInstanceScope->RegisterVar(new_var);

		}

	}

}

void ZClassNode::SetVars(std::vector<ZVarsNode*> vars)
{
	mVars = vars;
}

ZClassNode* ZClassNode::CreateInstance(std::string name) {

	ZClassNode* new_cls = new ZClassNode;

	new_cls->SetName(name);
	new_cls->SetMethods(mMethods);
	new_cls->SetVars(mVars);
	new_cls->CreateScope();
	new_cls->PopulateScope();
	new_cls->SetBaseName(this->mClassName);


	return new_cls;

}

ZMethodNode* ZClassNode::FindMethod(std::string name) {

	for (int i = 0; i < mMethods.size(); i++) {

		if (mMethods[i]->GetName() == name)
		{
			return mMethods[i];
		}

	}

	assert(false);
	return nullptr;

}

ZContextVar* ZClassNode::CallMethod(std::string name, std::initializer_list<ZContextVar*> args)
{
	auto method = FindMethod(name);

	return method->Exec({});
}


void ZClassNode::SetBaseName(std::string name) {

	mBaseClassName = name;

}