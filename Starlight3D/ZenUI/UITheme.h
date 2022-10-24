#pragma once
#include <string>
#include "UIColor.h"

class kFont;
class Texture2D;

class UITheme
{
public:

	UITheme(std::string path);
	
	Texture2D* mFrame;
	Texture2D* mButtonFrame;
	Texture2D* mFrameRounded;
	Texture2D* mWindowFrame;
	Texture2D* mWindowTitle;
	Texture2D* mWindowMain;

	UIColor mFrameColor;
	UIColor mButonColor;

	kFont* mMainFont;
	
	static UITheme* Theme;
};

