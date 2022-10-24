#include "pch.h"
#include "UIWindowContent.h"
#include "UIRenderer.h"
#include "Texture2D.h"
#include "UITheme.h"

void UIWindowContent::Render() {

	mUIRender->Draw(UITheme::Theme->mWindowFrame, GetRenderRect(), UITheme::Theme->mFrameColor);

	

}

void UIWindowContent::SetPage(WindowPage page) {

	mCurrentPage = page;
	mSub.resize(0);
	mSub.push_back(page.content);
	page.content->SetRoot(this);

}

WindowPage UIWindowContent::GetPage() {

	return mCurrentPage;

}