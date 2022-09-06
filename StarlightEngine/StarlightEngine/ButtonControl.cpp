#include "pch.h"
#include "ButtonControl.h"
#include "UI.h"

//#include "Texture2D.h"

#include "UITheme.h"




ButtonControl::ButtonControl() {

//	Back = new Texture2D("UI/Neon/buttongrey1.png");
	Over = false;
	Pressed = false;
	ControlAble = true;
};

void ButtonControl::Render() {

	
		float r, g, b, a;
		r = 0.7f;
		g = 0.7f;
		b = 0.7f;
		a = 1.0f;

		if (Over) {
			r = 2.2f;
			g = 2.2f;
			b = 2.2f;
			
		}

		if (Pressed)
		{
			r = 1.1;
			g = 2.1;
			b = 2.1;
		}

		Color = Helper::Lerp(Color, float3(r, g, b), 0.08f);
		std::cout << Color.r << std::endl;




		UI::Theme->DrawButton(GetX(), GetY(), GetW(), GetH(), GetText(), Color);
		//if (Highlight)
		{
		//	UI::Theme->DrawFrame(GetX(), GetY(), GetW(), 3, 0, 2, 2, 1);
		//	UI::Theme->DrawFrame(GetX(), GetY(), 3, GetH(), 0, 2, 2, 1);
		//	UI::Theme->DrawFrame(GetX(), GetY() + GetH() - 3, GetW(), 3, 0, 2, 2, 1);
	//		UI::Theme->DrawFrame(GetX() + GetW() - 3, GetY(), 3, GetH(), 0, 2, 2, 1);
		}
	//}
	//UI::DrawTexture(Back, GetX(), GetY(), GetW(),GetH(), 1, 1, 1, 1);
	
};

void ButtonControl::MouseEnter() {
	Over = true;
};

void ButtonControl::MouseLeave() {

	Over = false;

};

void ButtonControl::MouseDown(int b) {
	//exit(1);
	// 
	//exit(1);
	if (b == 0) {
		Pressed = true;
		Action();
		if (actData != NULL) {
			actData(Data);
		}
	}
};

void ButtonControl::MouseUp(int b) {
	//exit(1);
	Pressed = false;
}
