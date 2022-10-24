#include "pch.h"
#include "UIButton.h"
#include "UIRenderer.h"
#include "Texture2D.h"
#include "UITheme.h"
UIButton::UIButton() {

}

void UIButton::Render() {


	

	UIColor final_col = UITheme::Theme->mButonColor;

	final_col.Mix(mColor);

	mUIRender->Draw(UITheme::Theme->mButtonFrame, GetRenderRect(), final_col);
	int x, y;
	auto rect = GetRenderRect();

	x = rect.x + rect.w / 2;
	y = rect.y + rect.h / 2;

	x = x - UITheme::Theme->mMainFont->getWidth(mText.c_str())/2;
	y = y - UITheme::Theme->mMainFont->getHeight() / 2;


	mUIRender->Text(mText,UITheme::Theme->mMainFont, x, y, UIColor(1, 1, 1, 1));


	

}

void UIButton::Update() {

	if (mPressed) {
		mColor.Lerp(UIColor(3, 3, 3, 1), 0.25);
	}
	else {
		if (mHighlight) {
			mColor.Lerp(UIColor(2, 2, 2, 1), 0.15);
		}
		else {
			mColor.Lerp(UIColor(1, 1, 1, 1), 0.18);
		}
	}
}

void UIButton::MouseEnter() {

	mHighlight = true;

}

void UIButton::MouseLeave() {

	mHighlight = false;

}

void UIButton::MouseDown(int but) {
	mPressed = true;
}

void UIButton::MouseUp(int but)
{
	mPressed = false;
}

void UIButton::MouseMove(float2 pos,float2 mov) {
	if (mPressed) {

	//	UIRect r = GetRect();
	//	SetRect(UIRect(r.x + mov.x, r.y + mov.y, r.w, r.h));

	}
}