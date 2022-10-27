#include "pch.h"
#include "UIWindowContent.h"
#include "UIRenderer.h"
#include "Texture2D.h"
#include "UITheme.h"
#include "UIScrollerVert.h"


UIWindowContent::UIWindowContent() {

	mScrollerVert = new UIScrollerVert();
	Add(mScrollerVert);

}

void UIWindowContent::Update() {
	auto rect = GetRect();
	rect.x = rect.w - 16;
	rect.y = 0;
	rect.w = 16;

	mScrollerVert->SetRect(rect);
	mScrollerVert->SetMax(ContentHeight());
	int sy = mScrollerVert->GetY();
	printf("GetY:%d\n", sy);
	SetScroll(float2(0, sy));

}

void UIWindowContent::Render() {

	mUIRender->Draw(UITheme::Theme->mWindowFrame, GetRenderRect(), UITheme::Theme->mFrameColor);
	
}

void UIWindowContent::OnResize() {



}

void UIWindowContent::SetPage(WindowPage page) {

	mCurrentPage = page;
	mSub.resize(0);
	for (int i = 0; i < page.content->GetSub().size(); i++)
	{
		mSub.push_back(page.content->GetSub()[i]);
		page.content->GetSub()[i]->SetRoot(this);
	}
	
	//mSub.push_back(page.content);
	//page.content->SetRoot(this);



	if (!Contains(mScrollerVert)) {

		Add(mScrollerVert);
		//}
	}

}

WindowPage UIWindowContent::GetPage() {

	return mCurrentPage;

}