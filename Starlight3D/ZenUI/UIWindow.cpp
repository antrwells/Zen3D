#include "pch.h"
#include "UIWindow.h"
#include "UIRenderer.h"
#include "Texture2D.h"
#include "UITheme.h"

UIWindow::UIWindow(std::string title) {

	mText = title;
	mTitle = new UIWindowTitle(title);
	Add(mTitle);

	mContent = new UIWindowContent;
	mContent->SetPage(mTitle->GetPage(0));
	Add(mContent);

}


WindowPage UIWindow::AddPage(std::string title) {

	return mTitle->AddPage(title);

}

void UIWindow::SetPage(WindowPage page) {

	mContent->SetPage(page);

}

void UIWindow::OnResize() {

	auto title_rect = GetRect();

	title_rect.x = 0;
	title_rect.y = 0;

	title_rect.h = 32;
	mTitle->SetRect(title_rect);
	
	auto con_rect = GetRect();
	con_rect.x = 0;
	con_rect.y = 32;
	con_rect.h = con_rect.h - 32;


	mContent->SetRect(con_rect);

	
	int b = 5;


}

void UIWindow::Render() {

	auto rect = GetRenderRect();
	rect.x -= 3;
	rect.y -= 3;
	rect.w += 6;
	rect.h += 6;
	mUIRender->Draw(UITheme::Theme->mWindowMain,rect,UIColor(1,1,1,1));

}

WindowPage UIWindow::GetPage(int id)
{
	if (id == -1) {
		return mContent->GetPage();
	}
}