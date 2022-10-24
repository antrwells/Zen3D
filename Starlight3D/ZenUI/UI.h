#pragma once
#include <vector>
#include "Common/interface/BasicMath.hpp"
using namespace Diligent;

class UIBase;

class UI
{
public:

	UI();
	void SetRoot(UIBase* control);
	void Add(UIBase* base);

	//Update

	void Update();

	//Render

	void Render();

	std::vector<UIBase*> AddNode(std::vector<UIBase*> list,UIBase* base);
	std::vector<UIBase*> GetList(UIBase* base);
	std::vector<UIBase*> ReverseList(std::vector<UIBase*> list);



private:

	UIBase* mRoot = nullptr;
	UIBase* mHover = nullptr;
	UIBase* mPressed[32];

};

