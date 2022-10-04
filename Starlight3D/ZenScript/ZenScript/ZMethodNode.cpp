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
	//mCode->SetClassOwner(
	mCode->SetClassOwner(GetClassOwner());
	return mCode->Exec(params);
	//return nullptr;
}

void ZMethodNode::SetScope(ZContextScope* scope) {

	mScope = scope;

}

ZContextScope* ZMethodNode::GetScope() {

	return mScope;

}

void ZMethodNode::Bind() {

	mCode->SetClassOwner(GetClassOwner());


}