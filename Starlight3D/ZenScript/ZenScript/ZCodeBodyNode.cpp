#include "ZCodeBodyNode.h"


ZCodeBodyNode::ZCodeBodyNode() {

}

void ZCodeBodyNode::AddNode(ZScriptNode* node) {

	mNodes.push_back(node);

}

ZContextVar* ZCodeBodyNode::Exec(std::initializer_list<ZContextVar*> args)
{

	for (int i = 0; i < mNodes.size(); i++) {

		auto node = mNodes[i];

		node->Exec({});

	}

	return nullptr;

}