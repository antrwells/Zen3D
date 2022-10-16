#pragma once
#include <memory>
class TextureData
{
public:
	TextureData(int w,int h)
	{
		mData = (char*)malloc(w * h * 3);
	}

	char* mData;
	int mWidth, mHeight;
};

