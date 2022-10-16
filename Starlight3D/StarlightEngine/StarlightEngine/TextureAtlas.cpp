#include "pch.h"
#include "TextureAtlas.h"
#include "TextureData.h"

TextureAtlas::TextureAtlas(int width,int height) {


	rc = rect(0, 0, width, height);
	raw = new TextureData(width, height);

	//	topNode = new AtlasNode();
//	topNode->rect = float4(0, 0, mWidth, mHeight);

}

AtlasNode* TextureAtlas::Insert(int w,int h, int id =-1)
{

	if (root = nullptr) {
		//root = new AtlasNode()
		root = new AtlasNode(rect(0, 0, w, h), id);
		root->Used = true;
		root->child[0] = new AtlasNode(rect(0, h, rc.w, rc.h - h), -1);
		root->child[1] = new AtlasNode(rect(w, 0, rc.w - w, h), -1);
	}
	else {
		
	}

	return nullptr;

}
	