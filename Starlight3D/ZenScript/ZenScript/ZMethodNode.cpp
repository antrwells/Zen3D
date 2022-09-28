#include "ZMethodNode.h"
#include "ZSignatureNode.h"
#include "ZContextVar.h"
#include "ZCodeBodyNode.h"

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

std::string ZMethodNode::GetName() {

	return mMethodName;

}

ZContextVar* ZMethodNode::Exec(const std::vector<ZContextVar*>& params)
{
	return mCode->Exec(params);
	//return nullptr;
}