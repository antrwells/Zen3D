#include "ZVarsNode.h"

void ZVarsNode::SetType(VarType type) {

	mVarType = type;

}

void ZVarsNode::AddVar(std::string name) {

	mVarNames.push_back(name);

}

std::vector<std::string> ZVarsNode::GetNames() {

	return mVarNames;

}

VarType ZVarsNode::GetType() {

	return mVarType;

}