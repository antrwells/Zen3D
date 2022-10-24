#pragma once
#include <vector>
#include <functional>
#include "Texture2D.h"
//#include "DragObject.h"
#include "Helper.h"
#include<string>
#include "DockUsage.h"
class UIControl
{
public:
	UIControl();
	virtual void KeyPressed(int key,bool shift) {};
	virtual void Update() {};
	virtual void Render() {};
	virtual void KeyDown(int key) {};
	virtual void KeyUp(int key) {};
	virtual void MouseEnter() {};
	virtual void MouseLeave() {};
	virtual void MouseDown(int button) {  };
	virtual void MouseUp(int button) {};
	virtual void MouseMove(int x, int y, int dx, int dy) {};
	virtual void Resized() {  };
	virtual void AfterSet() {};
	virtual void DoubleClick(int b) {};
	Texture2D* GrabBackground();

	void Action();
	UIControl * Set(int x, int y, int w, int h);
	UIControl * SetText(const char* txt);
	int GetX();
	int GetY();
	int GetW();
	int GetH();
	int GetContentsW();
	int GetContentsH();
	int GetInternalX();
	int GetInternalY();
	int LocalX()
	{
		return Position.x;
	}
	int LocalY() {
		return Position.y;
	}
	const char* GetText();
	virtual bool InBounds(int x, int y);
	void AddControl(UIControl* control);
	void SetRoot(UIControl* root);
	int ControlsCount();
	bool GetActive() {
		return Active;
	};
	virtual void SetActive(bool active) {
		Active = active;
	};
	UIControl* GetControl(int index);
	void SetAction(std::function<void()> action)
	{
		act = action;
	}
	void SetSize(int w, int h)
	{
		Size = float2(w, h);
		int bb = 5;
		//W = w;
		//H = h;
		Resized();
	}
	void SetActionOne(std::function<void()> one) {
		actOne = one;
	};
	void SetDrag(std::function<void(int, int) > drag)
	{
		actDrag = drag;
	}

	virtual void Dragged(int x,int y)
	{
		if (actDrag != NULL) {
			actDrag(x, y);
		}
	}



	void SetX(int x) {
		Position.x = x;
		//X = x;
	};

	void SetY(int y) {
		Position.y = y;
	}
	void SetOffset(int x, int y) {
		OffX = x;
		OffY = y;
	}

	int GetOffX() {
		return OffX;
	}

	int GetOffY() {
		return OffY;
	}
	void SetCull(bool cull) {
		cullChild = cull;
	
	}
	bool GetCullChild() {
		return cullChild;
	}

	void ClearChild() {

		Controls.resize(0);

	}
	void SetOverride(int x, int y) {
		OverX = x;
		OverY = y;
	}
	bool CanDrag() {
		return canDrag;
	}
	bool CanDrop() {
		return canDrop;
	}
	void SetDrop(bool drop) {
		canDrop = drop;
	}
	void* GetData() {
		return Data;
	}
	void SetData(void* data) {
		Data = data;
	}
	void SetDropAct(std::function<void(void*)> act) {
		actDrop = act;
	}
	void DropAct(void* data) {
		actDrop(data);
	}
	virtual void Drop(UIControl* drop) {};
//	virtual void DropDrag(UIDragObject* obj) {};
	void SetActData(std::function<void(void*)> act) {
		actData = act;
	}
	bool IsControlAble() {
		return ControlAble;
	}
	void SetNext(UIControl* left, UIControl* right, UIControl* up, UIControl* down) {
		Up = up;
		Down = down;
		Left = left;
		Right = right;
	}
	UIControl* GetUp() {
		return Up;
	}
	UIControl* GetDown() {
		return Down;
	}
	UIControl* GetLeft() {
		return Left;
	}
	UIControl* GetRight() {
		return Right;
	}
	void SetToolTip(const char* txt) {
		ToolTip = txt;
	}
	const char* GetToolTip() {
		return ToolTip;
	}
	void SetHighlight(bool h) {
		Highlight = h;
	}
	float2 RenderPosition() {

		auto parent = float2(0, 0);
		
		if (Root != nullptr) {
			parent = Root->RenderPosition();
		}
		return parent + Position;

	}
	void DrawButton(float2 pos, float2 size, float4 col);
	void DrawFrame(float2 pos, float2 size, float4 col);
	void DrawWindowTitle(float2 pos, float2 size, float4 col);
	void DrawFrameRounded(float2 pos, float2 size, float4 col);
	float4 GetDragZone() {
		return mDragZone;
	}

protected:
	void (*actionPtr)();
	std::function<void()> act = NULL;
	void* Data;
	UIControl* Root = NULL;
	std::vector<UIControl*> Controls;
	const char* Text;
	//int X, Y, W, H;
	int OverX=-1, OverY=-1;
	int OffX, OffY;
	bool Active = false;
	std::function<void()> actOne;
	std::function<void(int, int)> actDrag;
	std::function<void(void*)> actDrop;
	std::function<void(void*)> actData = NULL;
	bool cullChild = false;
	bool canDrag = false;
	bool canDrop = false;
	bool ControlAble = false;
	UIControl* Up=NULL;
	UIControl* Left=NULL;
	UIControl* Right=NULL;
	UIControl* Down = NULL;
	const char* ToolTip = NULL;
	bool Highlight = false;
	Texture2D* BGTex = nullptr;
	std::string StdText;
	float4 Color;
	float2 Position;
	float2 Size;
	float4 mDragZone;
	DockUsage* mDockUsage;

};


