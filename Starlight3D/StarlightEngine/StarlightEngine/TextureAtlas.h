#pragma once
#include "AtlasNode.h"




class TextureData;

class TextureAtlas
{
public:
	TextureAtlas(int width, int height);
	AtlasNode* Insert(int w,int h,int id);
	AtlasNode* root;	
	rect rc;
	TextureData* raw;
};

