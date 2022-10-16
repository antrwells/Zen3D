#include "pch.h"
#include "AtlasNode.h"
#include "TextureData.h"

AtlasNode::AtlasNode(rect r, int id) {

	rc = r;
	child[0] = child[1] = nullptr;


}

AtlasNode* AtlasNode::Insert(int w, int h)
{

	if (Used) {
		AtlasNode* right = child[0]->Insert(w, h);
		if (right != nullptr) {
			return right;
		}
		AtlasNode* down = child[1]->Insert(w, h);
		if (down != nullptr) {
			return down;
		}
	}
	else {

		if (w <= rc.w && h <= rc.h)
		{
			Used = true;
			child[0] = new AtlasNode(rect(rc.x, rc.y + h, rc.w, rc.h - h),-1);
			child[1] = new AtlasNode(rect(rc.x + w, rc.y, rc.w - w, h),-1);
			rc.w = w;
			rc.h = h;
			return this;
		}
		else {
			return nullptr;
		}

	}
	return nullptr;

}