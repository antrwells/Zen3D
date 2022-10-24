#pragma once
#include "UIBase.h"
class UIButton : public UIBase
{
public:

	UIButton();
	void Update();
	void Render();
	void MouseEnter();
	void MouseLeave();
	void MouseMove(float2 pos,float2 delta);
	void MouseDown(int but);
	void MouseUp(int but);

private:

	bool mHighlight = false;
	bool mPressed = false;

};

