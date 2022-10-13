#include "ZProgramNode.h"


void ZMainNode::AddClass(ZClassNode* node) {

	mClasses.push_back(node);

}

void ZMainNode::AddStaticClass(ZClassNode* node)
{

	mStaticClasses.push_back(node);

}

std::vector<ZClassNode*> ZMainNode::GetClasses() {

	return mClasses;

}

std::vector<ZClassNode*> ZMainNode::GetStaticClasses() {

	return mStaticClasses;

}