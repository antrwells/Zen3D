#include "pch.h"
#include "GameUI.h"
#include "kFont.h"

GameUI::GameUI() {

	mDraw = new SmartDraw(Application::GetApp());
	UI = this;
	mButtonImg = new Texture2D("UI/ButtonFrame.png");
	mGameFont = new kFont("data/fonts/aqua.pf");
}

bool GameUI::Button(std::string text, int x, int y, int w, int h)
{

	float xp = (int)Application::GetApp()->GetInput()->GetMouseX();
	float yp = (int)Application::GetApp()->GetInput()->GetMouseY();

	if (xp >= x && xp <= x + w && yp >= y && yp <= y + h) {
		mDraw->DrawTexture(x, y, w, h, mButtonImg, 1.5f, 1.5f, 1.5f, 0.88f, false);
		if (Application::GetApp()->GetInput()->IsMouseDown(0))
		{
			return true;
		}
	}
	else {
		mDraw->DrawTexture(x, y, w, h, mButtonImg, 1, 1, 1, 0.88f, false);
	}
	mGameFont->drawText(text.c_str(), x + w / 2 - (mGameFont->getWidth(text.c_str()) / 2), y + h / 2-(mGameFont->getHeight()/2), 1, 1, 1, 1, mDraw);



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