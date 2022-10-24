#include "pch.h"
#include "UI.h"
#include "UIBase.h"
#include "UIRect.h"
#include "Application.h"
#include "UIImage.h"
#include "UITheme.h"
#include "UIRenderer.h"
UI::UI() {

	mRoot = new UIBase;

	UIRect rect(0, 0, Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight());
	mRoot->SetRect(rect);

	auto theme = new UITheme("Bright");
	for (int i = 0; i < 32; i++) {
		mPressed[i] = nullptr;
	}


}

//Graph

void UI::SetRoot(UIBase* control) {

	mRoot = control;

}

void UI::Add(UIBase* base) {

	mRoot->Add(base);

}

std::vector<UIBase*> UI::AddNode(std::vector<UIBase*> list, UIBase* base)
{

	list.push_back(base);
	for (int i = 0; i < base->GetSub().size(); i++)
	{
		auto sub = base->GetSub()[i];
		list = AddNode(list, sub);

	}

	return list;

}

std::vector<UIBase*> UI::GetList(UIBase* base) {

	std::vector<UIBase*> list;
	list = AddNode(list, base);
	return list;
}

std::vector<UIBase*> UI::ReverseList(std::vector<UIBase*> list)
{

	std::vector<UIBase*> rlist;
	std::vector<UIBase*> res;

	for (int i = 0; i < list.size(); i++)
	{
		int av = list.size() - (i+1);
		res.push_back(list[av]);
	}

	return res;

}

//update
void UI::Update() {

	auto list = GetList(mRoot);
	list = ReverseList(list);

	auto input = Application::GetApp()->GetInput();

	float2 mouse_pos = float2(input->GetMouseX(), input->GetMouseY());
	float2 mouse_del = float2(input->GetMouseDX(), input->GetMouseDY());
	bool but[32];
	but[0] = input->IsMouseDown(0);
	but[1] = input->IsMouseDown(1);

	bool found_over = false;

	for (int i = 0; i < list.size(); i++) {

		auto ui = list[i];
		ui->Update();

		if (mPressed[0] == nullptr)
		{
			if (!found_over) {
				if (ui->InBounds(mouse_pos))
				{
					if (mHover != ui) {
						if (mHover != nullptr) {
							mHover->MouseLeave();
						}
						mHover = ui;
						ui->MouseEnter();
					}
					else {
						mHover->MouseMove(mouse_pos,mouse_del);
					}
					mHover = ui;
					found_over = true;
				}
			}
		}
	}

	if (mHover != nullptr) {

		mHover->MouseMove(mouse_pos,mouse_del);

		if (mPressed[0] == nullptr) {
			if (but[0]) {

				mPressed[0] = mHover;
				mHover->MouseDown(0);
			}
		}
		if (mPressed[0] != nullptr) {

			if (but[0] == false)
			{
				mPressed[0]->MouseUp(0);
				mPressed[0] = nullptr;
			}

		}
		
	}
	

}

void UI::Render() {

	auto list = GetList(mRoot);

	UIBase::mUIRender->Begin();

	for (int i = 0; i < list.size(); i++) {

		auto ui = list[i];

		ui->Render();

	}

	UIBase::mUIRender->End();

}