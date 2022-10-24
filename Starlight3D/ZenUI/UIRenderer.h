#pragma once
#include "UIRect.h"
#include "UIColor.h"
#include <string>
#include "kFont.h"
class Texture2D;
class SmartDraw;


class UIRenderer
{
public:

	UIRenderer();
	void Begin();
	void Draw(Texture2D* img, UIRect rect,UIColor color);
	void Text(std::string text,kFont* font, int x, int y, UIColor color);
	void End();

private:

	SmartDraw* mDraw;

};

