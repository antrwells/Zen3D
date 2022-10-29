#pragma once
#include <string>
#include "SmartDraw.h"
#include "Texture2D.h"

/// <summary>
/// mini ui for in game user interfaces.
/// </summary>
/// cla
/// 
/// 
class kFont;

class GameUI
{
public:

	GameUI();
	void Begin();
	void End(float4x4 proj);
	bool Button(std::string text, int x, int y, int w, int h);
	void Image(Texture2D* img, int x, int y, int w, int h,float r,float g,float b,float a);
	static GameUI* UI;

private:
	SmartDraw* mDraw;
	Texture2D* mButtonImg;
	kFont* mGameFont;
};

