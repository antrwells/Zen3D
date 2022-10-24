#pragma once
#include "UIBase.h"
#include "WindowPage.h"

class UIWindowContent : public UIBase
{
public:

	void Render();
	void SetPage(WindowPage page);
	WindowPage GetPage();
private:

	WindowPage mCurrentPage;

};

