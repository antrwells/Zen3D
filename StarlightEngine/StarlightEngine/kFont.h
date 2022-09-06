#pragma once
//#include "kamGame.h"
#include "Texture2D.h"

//#include "kamGame.h"
#include <vector>
class SmartDraw;


class kFont
{
public:

	kFont(const char* path);
	void drawText(const char* text, int x, int y, float r, float g, float b, float a);
	void drawText(const char* text, int x, int y, float r, float g, float b, float a, SmartDraw* draw);
	int getWidth(const char* text);
	int getHeight();
	void setScale(float s)
	{
		scale = s;
	}
private:

	std::vector<Texture2D*> chars;
	//kamGame* game;
	float scale = 1.0;

};

