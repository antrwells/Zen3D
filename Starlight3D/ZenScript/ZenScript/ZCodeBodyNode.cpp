#include "ZCodeBodyNode.h"


ZCodeBodyNode::ZCodeBodyNode() {

}

void ZCodeBodyNode::AddNode(ZScriptNode* node) {

	mNodes.push_back(node);

}

ZContextVar* ZCodeBodyNode::Exec(const std::vector<ZContextVar*>& params)
{

	for (int i = 0; i < mNodes.size(); i++) {

		auto node = mNodes[i];

		node->Exec({});

	}

	return nullptr;

}

void ZCodeBodyNode::SetOwner(ZMethodNode* node) {

	mOwner = node;

}

ZMethodNode* ZCodeBodyNode::GetOwner() {

	return mOwner;

}