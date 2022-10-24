#include "pch.h"
#include "UIWindowTitle.h"
#include "UITheme.h"
#include "kFont.h"
#include "UIRenderer.h"
#include "UIWindow.h"

UIWindowTitle::UIWindowTitle(std::string title) {

	AddPage(title);

}

void UIWindowTitle::Render() {

	int dx = 5;

	mUIRender->Draw(UITheme::Theme->mWindowTitle, GetRenderRect(), mColor);

	
	auto rr = GetRenderRect();

	for (int i = 0; i < mPages.size(); i++) {

		auto page = mPages[i];

		int tab_width = UITheme::Theme->mMainFont->getWidth(page.title.c_str())+30;
		
		

		UIRect tab_rect;
		tab_rect.x = rr.x+dx;
		tab_rect.y = rr.y+0;
		tab_rect.w = tab_width;
		tab_rect.h = 28;

		UIColor color;

		int tx, ty;
		tx = tab_rect.w / 2;
		ty = tab_rect.h / 2;
		tx = tx - UITheme::Theme->mMainFont->getWidth(page.title.c_str()) / 2;
		ty = ty - UITheme::Theme->mMainFont->getHeight() / 2;
		
		if (mCurrentPage.id == page.id) {
			color = UIColor(2, 2, 2, 1);
		}

		mUIRender->Draw(UITheme::Theme->mButtonFrame, tab_rect, color);
		
		mUIRender->Text(page.title, UITheme::Theme->mMainFont,rr.x+ dx+tx,rr.y+ty, color);

		dx = dx + tab_width + 1;

	}

}

WindowPage UIWindowTitle::AddPage(std::string title) {

	WindowPage page;
	page.title = title;
	page.id = mPages.size() + 1;
	page.content = new UIBase;
	mPages.push_back(page);
	return page;

}

void UIWindowTitle::MouseMove(float2 pos, float2 delta) {

	mpos = pos;
	if (mMove) {
		auto root = GetRoot();
		UIRect r = root->GetRect();
		root->SetRect(UIRect(r.x + delta.x, r.y + delta.y, r.w, r.h));
	}

}

void UIWindowTitle::MouseUp(int but) {

	mMove = false;

}

void UIWindowTitle::MouseDown(int but)
{

	int dx = 5;

	//mUIRender->Draw(UITheme::Theme->mWindowTitle, GetRenderRect(), mColor);
	bool phit = false;
	auto rr = GetRenderRect();

	for (int i = 0; i < mPages.size(); i++) {

		auto page = mPages[i];

		int tab_width = UITheme::Theme->mMainFont->getWidth(page.title.c_str()) + 30;



		UIRect tab_rect;
		tab_rect.x = rr.x + dx;
		tab_rect.y = rr.y + 0;
		tab_rect.w = tab_width;
		tab_rect.h = 28;

		UIColor color;

		int tx, ty;
		tx = tab_rect.w / 2;
		ty = tab_rect.h / 2;
		tx = tx - UITheme::Theme->mMainFont->getWidth(page.title.c_str()) / 2;
		ty = ty - UITheme::Theme->mMainFont->getHeight() / 2;

		//mUIRender->Draw(UITheme::Theme->mButtonFrame, tab_rect, color);

		if (tab_rect.InBounds(mpos))
		{
			mCurrentPage = page;
			phit = true;
			auto win = (UIWindow*)GetRoot();
			win->SetPage(page);
		}

	//	mUIRender->Text(page.title, UITheme::Theme->mMainFont, rr.x + dx + tx, rr.y + ty, color);

		if (!phit) {
			mMove = true;
		}
		else {
			mMove = false;
		}
		dx = dx + tab_width + 1;

	}

}

WindowPage UIWindowTitle::GetPage(int id)
{

	return mPages[id];

}