#pragma once

#include "UIRect.h"
#include "UIColor.h"
#include <vector>
#include <string>
#include "Common/interface/BasicMath.hpp"
using namespace Diligent;
class UIRenderer;
class Texture2D;

class UIBase
{
public:

	UIBase();
	virtual void Update() {};
	virtual void Render();

	//Render
	void SetRect(UIRect rect);
	UIRect GetRenderRect();
	UIRect GetRenderRectNoScroll();
	UIRect GetRect();
	int ContentHeight();

	//Graph
	UIBase* GetRoot();
	void Add(UIBase* ui);
	void SetRoot(UIBase* base);
	bool Contains(UIBase* base);
	float2 GetScroll();
	void SetScroll(float2 scroll);

	std::vector<UIBase*> GetSub();

	//events
	virtual void MouseEnter() {};
	virtual void MouseLeave() {};
	virtual void MouseMove(float2 pos,float2 delta) {};
	virtual void MouseDown(int but) {};
	virtual void MouseUp(int but) {};
	virtual void OnResize() {};


	//look
	void SetImage(Texture2D* image);

	//static
	static UIRenderer* mUIRender;

	//interactivity
	bool InBounds(float2 mouse);

	void SetText(std::string text) {
		mText = text;
	}
	bool IsContent() {
		return mIsContent;
	}
protected:

	//Graph related
	UIRect mRect;
	UIBase* mRoot;
	std::vector<UIBase*> mSub;

	//Look related
	UIColor mColor;
	Texture2D* mImage = nullptr;

	//Details related
	std::string mName = "";
	std::string mText = "";
	bool mCanScroll = true;
	float2 mScroll;
	bool mIsContent = true;


};

