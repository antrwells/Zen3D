#pragma once
#include "UIControl.h"
#include "WindowContent.h"
#include <vector>

class WindowControl : public UIControl
{
public:

	WindowControl(const char* buf);
	void Render();
	void AddContent(WindowContent* content);
	void MouseEnter();
	void MouseLeave();
	void MouseMove(int x, int y, int dx, int dy);
	void Dragged(int x, int y);
	void Resized() {
		mDragZone = float4(0, 34, Size.x, Size.y);
	}
private:
	std::vector<WindowContent*> mContent;
	WindowContent* mActiveContent;
	WindowContent* mContentHover;

};

