#include "ZMethodNode.h"
#include "ZSignatureNode.h"

void ZMethodNode::SetName(std::string name)
{

	mMethodName = name;

}

void ZMethodNode::SetSignature(ZSignatureNode* node)
{

	mSignature = node;

}

void ZMethodNode::SetReturnType(VarType type) {

	mReturnType = type;

}

void ZMethodNode::SetCode(ZCodeBodyNode* code)
{

	mCode = code;

}