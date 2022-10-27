#include "pch.h"
#include "UIBase.h"
#include "UIRenderer.h"
#include "Texture2D.h"

UIBase::UIBase() {

	if (mUIRender == nullptr) {

		mUIRender = new UIRenderer;

	}

	mImage = new Texture2D("UI/Theme/Bright/FrameSquare.png");
	mScroll = float2(0, 0);
}

void UIBase::SetRect(UIRect rect) {

	mRect = rect;
	OnResize();




}

//Render

UIRect UIBase::GetRenderRectNoScroll()
{

	UIRect origin;

	if (mRoot != nullptr) {

		origin = mRoot->GetRenderRectNoScroll();

	}

	origin.x += mRect.x;
	origin.y += mRect.y;



	origin.w = mRect.w;
	origin.h = mRect.h;

	return origin;

}

int UIBase::ContentHeight() {

	int by = 0;
	for (int i = 0; i < mSub.size(); i++) {

		if (mSub[i]->IsContent()) {
			int cy = mSub[i]->GetRect().y;// +mSub[i]->GetRect().h;
			if (cy > by) {
				by = cy;
			}
		}

	}
	return by;
}

UIRect UIBase::GetRenderRect() {

	UIRect origin;

	if (mCanScroll == false) {

		return GetRenderRectNoScroll();

	}

	if (mRoot != nullptr) {

		origin = mRoot->GetRenderRect();

	}

	origin.x += mRect.x;
	origin.y += mRect.y;

	if (mCanScroll) {

		if (mRoot != nullptr) {
						
			float2 sv = mRoot->GetScroll(); 
			if (sv.y != 0)
			{
				int aa = 5;
			}
			origin.y -= sv.y;
			origin.x -= sv.x;
		}

	}
	else {

		int aa = 5;
	}

	origin.w = mRect.w;
	origin.h = mRect.h;

	return origin;

}

float2 UIBase::GetScroll() {

	return mScroll;

}

void UIBase::SetScroll(float2 scroll) {

	mScroll = scroll;

}

//Scene
UIBase* UIBase::GetRoot() {

	return mRoot;

}

std::vector<UIBase*> UIBase::GetSub() {
	
	return mSub;

}

void UIBase::Add(UIBase* base) {

	base->SetRoot(this);
	mSub.push_back(base);

}

void UIBase::SetRoot(UIBase* base) {

	mRoot = base;

}

bool UIBase::Contains(UIBase* base) {

	for (int i = 0; i < mSub.size(); i++) {
		if (base == mSub[i])
		{
			return true;
		}
	}
	return false;

}

//Render

void UIBase::Render() {

	

}

//look

void UIBase::SetImage(Texture2D* image) {

	mImage = image;

}


bool UIBase::InBounds(float2 mouse)
{

	auto rect = GetRenderRect();

	if (mouse.x > rect.x && mouse.x < rect.x + rect.w)
	{
		if (mouse.y > rect.y && mouse.y < rect.y + rect.h)
		{
			return true;
		}
	}

	return false;

}

UIRect UIBase::GetRect() {

	return mRect;

}

//Static

UIRenderer* UIBase::mUIRender = nullptr;