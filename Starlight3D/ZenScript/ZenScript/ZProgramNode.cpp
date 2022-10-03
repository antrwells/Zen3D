#include "ZProgramNode.h"


void ZMainNode::AddClass(ZClassNode* node) {

	mClasses.push_back(node);

}

std::vector<ZClassNode*> ZMainNode::GetClasses() {

	return mClasses;

}