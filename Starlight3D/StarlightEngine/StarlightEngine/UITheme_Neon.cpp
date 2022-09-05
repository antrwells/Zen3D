#include "pch.h"
#include "UITheme_Neon.h"
#include "Texture2D.h"
#include "UI.h"
UITheme_Neon::UITheme_Neon() {


	But = new Texture2D("data/ui/theme/darktheme/button1.png");

	//ButDown = new Texture2D("UI/Neon/ButDown.png");
	//ButLeft = new Texture2D("UI/Neon/ButLeft.png");
	//ButRight = new Texture2D("UI/Neon/ButRight.png");

	//ButLeftTop = new Texture2D("UI/Neon/butlefttop.png");
	//ButLeftDown = new Texture2D("UI/Neon/ButLeftDown.png");
		
	//ButRightTop = new Texture2D("UI/Neon/butrighttop.png");
	//ButRightDown = new Texture2D("UI/Neon/Butrightdown.png");
	
	//ButMid = new Texture2D("edit/button1.png");

	//FrameBG = new Texture2D("edit/frame.png");

	//ButMid = FrameBG;



	//while (true) {};

};

void UITheme_Neon::DrawButton(int x, int y, int w, int h, const char* text,bool over,bool pressed)
{


	DrawImg(x, y, w, h, But, 1, 1, 1, 1);
	UI::RenderText(x + w / 2 - (UI::TextWidth(text) / 2), y + h / 2 - (UI::TextHeight(text) / 2), text, 1, 1, 1, 1);
	return;
	if (pressed) {
		//DrawDyanmicBox(x, y, w, h, ButTop, ButDown, ButLeft, ButRight, ButLeftTop, ButLeftDown, ButRightTop, ButRightDown, ButMid, 0.8, 1, 1, 1);
		//DrawImg(x, y, w, h, ButMid,1.3f, 1.3f, 1.3f, 1);
	}else if (over) {
		//DrawDyanmicBox(x, y, w, h, ButTop, ButDown, ButLeft, ButRight, ButLeftTop, ButLeftDown, ButRightTop, ButRightDown, ButMid, 0.6f, 0.9f, 0.9f, 1);
		//DrawImg(x, y, w, h, ButMid, 1, 1, 1, 1.0f);
	}
	else {
		//DrawDyanmicBox(x, y, w, h, ButTop, ButDown, ButLeft, ButRight, ButLeftTop, ButLeftDown, ButRightTop, ButRightDown, ButMid, 0.5f, 0.7f, 0.7f, 1);
		//DrawImg(x, y, w, h, ButMid, 0.8f, 0.8f, 0.8f, 1.0f);
	}
	//printf(text);
	//
//	printf(text);
	//printf("W:%d H:%D tw:%d", w, 0, UI::TextWidth(text));
	//printf("\n");

	//UI::RenderText(x + w / 2 - (UI::TextWidth(text) / 2)+2, y + h / 2 - (UI::TextHeight(text)/2)+2, text, 0, 0, 0, 1);
	//UI::RenderText(x + w / 2 - (UI::TextWidth(text) / 2), y + h / 2 - (UI::TextHeight(text) / 2), text, 1, 1, 1, 1);


};



void UITheme_Neon::DrawFrame(int x, int y, int w, int h,float r,float g,float b,float a) {

	UI::DrawTexture(FrameBG, x, y, w, h, r, g, b, a);


}