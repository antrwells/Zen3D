#pragma once

enum KeyID {

	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, Space, Return, Backspace, Delete,None,Shift

};

class UserInput
{
public:
	UserInput() {
		for (int i = 0;i < 512;i++) {
			KeyIn[i] = false;
		}
		for (int i = 0;i < 16;i++) {
			mMouseDown[i] = false;
		}
	}
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

	void SetKey(KeyID id, bool press) {
		KeyIn[id] = press;
	}

	bool IsKeyDown(KeyID id) {
		return KeyIn[(int)id];
	}

	bool IsMouseDown(int id) {
		return mMouseDown[id];
	}

	void SetMouseDown(int id, bool state) {
		mMouseDown[id] = state;
	}

private:
	int mMouseX, mMouseY;
	int mDeltaX, mDeltaY;
	bool KeyIn[512];
	bool mMouseDown[64];
};

