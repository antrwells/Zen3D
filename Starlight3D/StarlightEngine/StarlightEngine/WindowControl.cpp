#include "pch.h"
#include "WindowControl.h"
#include "UI.h"

WindowControl::WindowControl(const char* title) {

	mContent.resize(0);
	WindowContent* main = new WindowContent(title);
	WindowContent* test2 = new WindowContent("Other stuff.");
	WindowContent* test3 = new WindowContent("3D settings");
	//mContent.push_back(main);
	AddContent(main);
	AddContent(test2);
	AddContent(test3);
	mActiveContent = test2;
	
	//mDragZone = float4(0,0)


}

void WindowControl::AddContent(WindowContent* content) {

	mContent.push_back(content);

}

void WindowControl::Render() {

	Color = float4(1, 1, 1, 1);

	DrawWindowTitle(float2(0, 0), float2(Size.x, 34), Color);
	DrawFrame(float2(0, 34), float2(Size.x, Size.y - 32), Color);

	int cx = 0;

	for (int i = 0;i < mContent.size();i++) {

		auto content = mContent[i];

		int text_width = UI::TextWidth(content->GetTitle()) + 16;

		if (content == mActiveContent) {

			DrawFrame(float2(cx, 0), float2(text_width + 8, 36), float4(1, 1, 1, 1));

		}
		if (content == mContentHover) {

			DrawFrame(float2(cx, 0), float2(text_width + 8, 36), float4(0, 2, 2, 1));
		}


		DrawFrameRounded(float2(cx + 4, 2), float2(text_width, 34), float4(1, 1, 1, 1));
		UI::RenderText(RenderPosition().x + cx + 10, RenderPosition().y + 13, content->GetTitle(), 1, 1, 1, 1);

		cx = cx + text_width + 6;

	}

}

void WindowControl::MouseEnter() {

}

void WindowControl::MouseLeave() {


}

void WindowControl::MouseMove(int x, int y, int dx, int dy)
{

	int cx = 0;
	mContentHover = nullptr;
	for (int i = 0;i < mContent.size();i++) {

		auto content = mContent[i];

		int text_width = UI::TextWidth(content->GetTitle()) + 16;

		if (content == mActiveContent) {
			//DrawFrame(float2(cx, 0), float2(text_width + 8, 36), float4(0, 0, 0, 1));
		}
		//DrawFrameRounded(float2(cx + 4, 2), float2(text_width, 34), float4(1, 1, 1, 1));
		//UI::RenderText(cx + 10, 13, content->GetTitle(), 1, 1, 1, 1);
		if (x >= cx && x <= cx + text_width + 4 && y >= 0 && y <= 32) {
			mContentHover = content;
			return;

		}

		cx = cx + text_width + 6;

	}

	printf("X:%d Y:%d XD:%d YD:%d \n", x, y, dx, dy);


}

void WindowControl::Dragged(int x, int y) {

	
	Position = Position + float2(x, y);


}