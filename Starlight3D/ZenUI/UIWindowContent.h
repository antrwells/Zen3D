#pragma once
#include "UIBase.h"
#include "WindowPage.h"

class UIScrollerVert;

class UIWindowContent : public UIBase
{
public:
	UIWindowContent();
	void Render();
	void Update();
	void SetPage(WindowPage page);
	void OnResize();
	WindowPage GetPage();
private:

	WindowPage mCurrentPage;
	UIScrollerVert* mScrollerVert;
};

