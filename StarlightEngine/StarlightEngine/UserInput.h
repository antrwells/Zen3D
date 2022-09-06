#pragma once
class UserInput
{
public:

	void SetMouse(int mx, int my, int dx, int dy) {
		mMouseX = mx;
		mMouseY = my;
		mDeltaX = dx;
		mDeltaY = dy;
	}

	int GetMouseX() {
		return mMouseX;
	}
	int GetMouseY() {
		return mMouseY;
	}

	int GetMouseDX() {
		return mDeltaX;
	}

	int GetMouseDY() {
		return mDeltaY;
	}

private:
	int mMouseX, mMouseY;
	int mDeltaX, mDeltaY;
};

