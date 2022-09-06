#pragma once
#include "UITheme.h"

class Texture2D;

class UITheme_Neon : public UITheme
{
public:

	UITheme_Neon();

	void DrawButton(int x, int y, int w, int h, const char* text,float3 color);
	void DrawFrame(int x, int y, int w, int h, float r, float g, float b, float a);

private:

	

};


