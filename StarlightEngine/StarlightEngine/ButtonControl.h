#pragma once
#include "UIControl.h"

class Texture2D;

class ButtonControl : public UIControl
{
public:
	ButtonControl();
	void Render();
	void MouseEnter();
	void MouseLeave();
	void MouseDown(int b);
	void MouseUp(int b);
	void SetIcon(Texture2D* icon) {
		Icon = icon;
	}

private:
	bool Over, Pressed;
	Texture2D* Back;

	Texture2D* Icon = NULL;

};

