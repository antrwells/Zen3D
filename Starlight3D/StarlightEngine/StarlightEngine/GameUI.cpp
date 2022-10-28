#include "pch.h"
#include "GameUI.h"


GameUI::GameUI() {

	mDraw = new SmartDraw(Application::GetApp());
	UI = this;
	mButtonImg = new Texture2D("UI/ButtonFrame.png");
}

bool GameUI::Button(std::string text, int x, int y, int w, int h)
{

	mDraw->DrawTexture(x, y, w, h, mButtonImg, 1, 1, 1, 1, false);

	return false;
}

void GameUI::Image(Texture2D* img, int x, int y, int w, int h,float r,float g,float b,float a)
{

	mDraw->DrawTexture(x, y, w, h,img, r,g,b,a, false);

}

void GameUI::Begin() {

	mDraw->Begin();

}

void GameUI::End(float4x4 proj) {

	mDraw->End(proj);

}


GameUI* GameUI::UI = nullptr;