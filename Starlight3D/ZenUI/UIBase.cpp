#include "pch.h"
#include "UIBase.h"
#include "UIRenderer.h"
#include "Texture2D.h"

UIBase::UIBase() {

	if (mUIRender == nullptr) {

		mUIRender = new UIRenderer;

	}

	mImage = new Texture2D("UI/Theme/Bright/FrameSquare.png");

}

void UIBase::SetRect(UIRect rect) {

	mRect = rect;
	OnResize();




}

//Render

UIRect UIBase::GetRenderRect() {

	UIRect origin;

	if (mRoot != nullptr) {

		origin = mRoot->GetRenderRect();

	}

	origin.x += mRect.x;
	origin.y += mRect.y;

	origin.w = mRect.w;
	origin.h = mRect.h;

	return origin;

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