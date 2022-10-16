#pragma once

#include "Common/interface/BasicMath.hpp"
using namespace Diligent;

class TextureData;

struct rect {

	int x, y, w, h;
	rect(int rx, int ry, int rw, int rh)
	{
		x = rx;
		y = ry;
		w = rw;
		h = rh;
	}

	rect()
	{
		x = y = w = h = 0;
	}

};

class AtlasNode
{
public:
	
	AtlasNode(rect rc, int id);
	AtlasNode* child[2];
	rect rc = rect(0, 0, 0, 0);
	int ImageID = 0;
	TextureData* img = nullptr;
	bool Used = false;
	AtlasNode* Insert(int w, int h);
	//AtlasNode(rect rc, int id);
};

