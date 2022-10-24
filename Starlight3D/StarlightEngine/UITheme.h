#pragma once
#include "Texture2D.h"
#include "UI.h"
class Texture2D;

class UITheme
{
public:

	virtual void DrawButton(int x, int y, int w, int h, const char* text,float3 color) {};
	virtual void DrawFrame(int x, int y, int w, int h, float r, float g, float b, float a) {};
	virtual void DrawImg(int x, int y, int w, int h, Texture2D* tex, float r, float g, float b, float a)
	{
		UI::DrawTexture(tex, x, y, w, h, r, g, b, a);
	}

	Texture2D* GetButton() {
		return But;
	}

	Texture2D* GetDragger() {
		return Dragger;
	}

	Texture2D* GetFrame() {
		return Frame;
	}

	Texture2D* GetFrameRounded() {
		return FrameRounded;
	}

	Texture2D* GetLine() {

		return Line;

	}

	Texture2D* GetWindowTitle() {
		return WindowTitle;
	}

	//void DrawDyanmicBox(int x, int y, int w, int h, Texture2D* t, Texture2D* d, Texture2D* l,Texture2D* r, Texture2D* lt,Texture2D* ld, Texture2D* rt, Texture2D* rd,Texture2D* mid,float cr,float cg,float cb,float ca);

	Texture2D* But;
	Texture2D* Dragger;
	Texture2D* Frame;
	Texture2D* FrameRounded;
	Texture2D* Line;
	Texture2D* WindowTitle;


};

