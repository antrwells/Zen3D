#pragma once

class UIControl;

class DockUsage
{
public:

	DockUsage();
	void SetLeft(UIControl* control);
	void SetRight(UIControl* control);
	void SetDown(UIControl* control);
	void SetUp(UIControl* control);
	void SetDoc(UIControl* control);

private:

	bool mLeft, mRight, mUp, mDown, mDoc;
	UIControl* mLeftControl, * mRightControl, * mUpControl, * mDownControl, * mDocControl;

};

