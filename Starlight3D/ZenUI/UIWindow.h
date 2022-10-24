#pragma once
#include "UIBase.h"
#include "UIWindowTitle.h"
#include "UIWindowContent.h"

class UIWindow : public UIBase
{
public:

	UIWindow(std::string name);
	void OnResize();
	void Render();
	WindowPage GetPage(int id = -1);
	WindowPage AddPage(std::string title);
	void SetPage(WindowPage page);

private:

	UIWindowTitle* mTitle;
	UIWindowContent* mContent;

};

