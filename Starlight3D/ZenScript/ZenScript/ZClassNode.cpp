#include "ZClassNode.h"
#include "ZMethodNode.h"
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


ZClassNode* ZClassNode::CreateInstance(std::string name) {

	ZClassNode* new_cls = new ZClassNode;
	new_cls->SetName(name);
	new_cls->SetMethods(mMethods);

	return new_cls;


}