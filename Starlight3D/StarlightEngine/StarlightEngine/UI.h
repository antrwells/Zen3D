#pragma once
#include "UIControl.h"
//#include "MainMenuControl.h"
//#include "ControlInput.h"
//#include "DragObject.h"
#include "UserInput.h"
#include "Texture2D.h"
#include "SmartDraw.h"
//#include "UITheme.h"
#include "kFont.h"

class IDraw;
//class Texture2D;

class UITheme;
//class kFont;




struct CursorPing
{

	float x, y, r;

};

class UI
{
public:

	UI(int w, int h);
	UIControl* GetRoot();
	void Update();
	void Render();
	void RenderControl(UIControl* control);
	bool UpdateControl(UIControl* control);
	UIControl* GetOver() {
		return UIOver;
	}
	void SetTop(UIControl* control) {

		Top = control;

	};
	static void DrawTexture(Texture2D* tex, int x, int y, int w, int h, float r, float g, float b, float a,SmartDraw* draw);
	static void DrawTexture(Texture2D* tex, int x, int y, int w, int h, float r, float g, float b, float a);
	static SmartDraw* Drawer;
	static UITheme* Theme;
	static void SetTheme(UITheme* theme);
	static void SetMouse(int mx, int my, int mz);
	static void SetMouseBut(int id, bool state);
	static void SetKey(int id, bool state);
	static void CallKey(int id);
	static void SetShift(bool shift) {
		ShiftOn = shift;
	}
	static kFont* UIFont;

	static void RenderText(int x, int y, const char* text, float r, float g, float b, float a, IDraw* draw);
	static void RenderText(int x, int y, const char* text, float r, float g, float b, float a);
	static int TextWidth(const char* text);
	static int TextHeight(const char* text);
	int GetW() {
		return UW;
	}
	int GetH() {
		return UH;
	}

	//void SetMenu(MainMenuControl* menu) {
//		MainMenu = menu;
	//}

	UIControl* GetActive() {

		return UIActive;
	}
	bool GetBut(int i) {
		return But[i];
	}
	static UI* Main;
	int GetMouseX() {
		return MouseX;
	}
	int GetMouseY() {
		return MouseY;
	}
	void SetMenu(UIControl* menu) {
		MainMenu = menu;
	}
//	void SetDragObject(UIDragObject* obj) {
//		CurrentDrag = obj;
//	}
	void SetInput(UserInput* input) {
		Input = input;
	}
private:
	UIControl* Top = NULL;
	UIControl* UIRoot;
	UIControl* MainMenu;
	Texture2D* CursorTex;
	Texture2D* White;
	static int MouseX, MouseY, MouseZ;
	static bool But[32];
	static bool Key[255];
	UIControl* UIOver;
	UIControl* UIActive;
	UIControl* UIPressed[32];
	UIControl* Menu = NULL;
	int PressedBut;
	static int LastX, LastY;
	int UW, UH;
	std::vector<CursorPing> pings;
	UIControl* Dragging = NULL;
	static bool ShiftOn;
	UIControl* JoyActive = NULL;
	bool ControllerMode = true;
	UserInput* Input;
	int toolTipStart = -1;
	//UIDragObject* CurrentDrag = NULL;

};

