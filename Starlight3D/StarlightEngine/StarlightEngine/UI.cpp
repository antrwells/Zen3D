#include "pch.h"
#include "UI.h"
#include "GLFW/glfw3.h"
//#include "IntelliDraw.h"

#include "Texture2D.h"
//#include "UITheme.h"
//#include "kFont.h"
//#include "VString.h"
//#include "Input.h"
#include "Application.h"
UI::UI(int w,int h) {

	UIRoot = new UIControl();

	Drawer = new SmartDraw(Application::GetApp());
	//CursorTex = new Kinetic::Textures::Texture2D("me/ui/cursor.png", true);
	
	CursorTex = new Texture2D("data/ui/cursor.png");

//	White = new Kinetic::Textures::Texture2D("me/ui/white1.png", false);
	MouseX = 1;
	MouseY = 1;
	MouseZ = 0;

	UIOver = NULL;
	for (int i = 0; i < 16; i++) {
		UIPressed[i] = NULL;
	}
	UIActive = NULL;
	PressedBut = -1;
	LastX = 0;
	LastY = 0;
	UW = w;
	UH = h;

	//UIFont = new kFont("font/font.pf");
	Main = this;
	//Control = new ControlInput();

}

SmartDraw * UI::Drawer = NULL;

UITheme* UI::Theme = NULL;

UI* UI::Main = NULL;

void UI::SetTheme(UITheme* theme) {
	UI::Theme = theme;
}

UIControl* UI::GetRoot() {

	return UIRoot;

}

bool firstKey = false;
int firstTime = 0;

bool pinged = false;

int lkey = -1;

void UI::Update() {


	//***
	//SetMouse(Kinetic::Input::Input::MouseX, Kinetic::Input::Input::MouseY, 0);
	SetMouse(Input->GetMouseX(), Input->GetMouseY(), 0);
	//printf("Begin update\n");
	//return;
	if (But[0]) {
		if (!pinged) {

			CursorPing ping;
			ping.x = MouseX;
			ping.y = MouseY;
			ping.r = 1.0f;
			pings.push_back(ping);
			pinged = true;
		}
	}
	else {
		pinged = false;
	}
	//Control->Update();
	if (Top != NULL) {
		if (!UpdateControl(Top)) {

			UpdateControl(UIRoot);
		}
	}
	else {
		UpdateControl(UIRoot);
	}

	if (MainMenu != NULL) {
		UpdateControl(MainMenu);
	}


//	printf("End update\n");

	for (int i = 0; i < 16; i++) {
		if (UIPressed[i] != NULL) {

			if (But[i] == false) {

				UIPressed[i]->MouseUp(i);
				if (UIOver != NULL) {
					if (UIOver != Dragging) {
						if (Dragging != NULL) {
							if (UIOver->CanDrop())
							{
								UIOver->Drop(Dragging);
								UIOver->DropAct(Dragging->GetData());
							}
						}
					}
				}
				Dragging = NULL;
				UIPressed[i] = NULL;


			}

		}
	}

	if (UIOver != NULL) {

	//	printf("Over:%s", UIOver->GetText());
		bool keep = false;

		for (int cx = 0; cx < 16; cx++) {

			if (UIPressed[cx] == UIOver) {

				keep = true;

			}

		}

		if (!keep) {

			if (!UIOver->InBounds(MouseX, MouseY))
			{
				UIOver->MouseLeave();
				UIOver = NULL;

			}

		}

		

	};

	for (int i = 0; i < 16; i++) {
		if (UIPressed[i] != NULL) {

			int xd = UI::MouseX - UI::LastX;
			int yd = UI::MouseY - UI::LastY;
			UIPressed[i]->Dragged(xd, yd);

		}


		if (UIPressed[i] != NULL) {
			if (UIOver != NULL) {
				int mx = UI::MouseX - UIOver->GetX();
				int my = UI::MouseY - UIOver->GetY();
				UIPressed[i]->MouseMove(mx, my, UI::MouseX - UI::LastX, UI::MouseY - UI::LastY);
			}
		}
	}
	
	if (UIOver != NULL) {

		bool dol = false;
		for (int i = 0; i < 16; i++){
			if (UIPressed[i] == UIOver) {
				dol = true;
			}
		}

		if (UI::But[0]) {
			if (UIActive != NULL && UIOver != UIActive) {

				UIActive->SetActive(false);

			}
			UIActive = UIOver;
			UIOver->SetActive(true);

		}

		if (dol == false) {
			int mx = UI::MouseX - UIOver->GetX();
			int my = UI::MouseY - UIOver->GetY();

			UIOver->MouseMove(mx, my, UI::MouseX - UI::LastX, UI::MouseY - UI::LastY);
			
		}

	}

	if (UIActive != nullptr)
	{

		/// <summary>
		/// ***
		/// </summary>
		/*
		bool skey = Kinetic::Input::Input::KeyState[GLFW_KEY_LEFT_SHIFT];
		if (Kinetic::Input::Input::KeyState[GLFW_KEY_RIGHT_SHIFT]) {

			skey = true;

		}



		if (Kinetic::Input::Input::KeyPressed()) {


			for (int i = 0; i < 555; i++) {

				if (i == GLFW_KEY_LEFT_SHIFT || i == GLFW_KEY_RIGHT_SHIFT) {

					continue;

				}

				if (Kinetic::Input::Input::KeyState[i])
				{

					if (i != lkey) {

						firstKey = true;
						

					}

				}

			}

			if (firstKey) {

				firstKey = false;

				for (int i = 0; i < 555; i++) {

					bool ks = Kinetic::Input::Input::KeyState[i];

					

					if (ks) {
						lkey = i;
						UIActive->KeyPressed(i, skey);
						break;

					}

				}

				firstTime = clock() + 650;

			}
			else if (clock() > firstTime)
			{

//				if ((clock() - firstTime) > 450) {



//				}

				for (int i = 0; i < 555; i++) {

					bool ks = Kinetic::Input::Input::KeyState[i];
					if (ks) {

						UIActive->KeyPressed(i, skey);
						lkey = i;
						break;

					}

				}

			}
		}
		else {
			firstKey = true;
			//firstTime = 0;

		}
			*/
	}
	
	UI::LastX = UI::MouseX;
	UI::LastY = UI::MouseY;
};

int lClick = 0;

bool UI::UpdateControl(UIControl* control) {

//	printf("Updating:%s\n", control->GetText());
//	printf("GX:%d GY:%d W:%d H:%d", control->GetX(), control->GetY(), control->GetW(), control->GetH());
	for (int i = 0; i < control->ControlsCount(); i++) {

		int reali = control->ControlsCount() - (i + 1);
		if (UpdateControl(control->GetControl(reali))) {
			return true;
		}

	}

//	printf("Updating internal:%s\n", control->GetText());
	control->Update();


	if (JoyActive == NULL) {

		if (control->IsControlAble()) {

			JoyActive = control;

		}

	}

	UIControl* ja = JoyActive;

	/*

	if (Control->IsPressed(PadButton::Up)) {


		JoyActive = control->GetUp();
		exit(5);

	}

	if (Control->IsPressed(PadButton::Down)) {

		JoyActive = control->GetDown();
	}

	if (Control->IsPressed(PadButton::Left)) {
		JoyActive = control->GetLeft();
	}

	if (Control->IsPressed(PadButton::Right)) {
		JoyActive = control->GetRight();
	}

	if (JoyActive == NULL) {

		JoyActive = ja;

	}


	if (JoyActive != NULL) {
		if (JoyActive->IsControlAble() == false) {
			JoyActive = NULL;
		}
	}

	if (JoyActive != NULL) {

		if (Control->IsPressed(PadButton::A))
		{
			JoyActive->MouseDown(0);
		}

	}
	*/

	if (control->InBounds(MouseX, MouseY))
	{
	//	printf("in bounds.\n");
	//	printf("Control:");
	//	printf(control->GetText());
	//	printf("\n");
	//	printf("X:%d Y:%d GX:%d GY:%d\n", control->LocalX(), control->LocalY(), control->GetX(), control->GetY());	
		
		if (UIOver != NULL) {

	//		printf("MX:%d MY:%d\n", MouseX, MouseY);
			if (UIOver != control) {
			//	printf("Over new control.============================================================\n");
				UIOver->MouseLeave();
			//	UIOver->SetActive(false);
				UIOver = control;
				UIOver->MouseEnter();
				

			}
		}
		else {
			control->MouseEnter();
			UIOver = control;
		//	printf("Over new control.============================================================\n");
			if (UI::But[0]) {
			//	UIOver->SetActive(true);
			}
		}


		for (int i = 0; i < 16; i++) {
			if (But[i]) {

				if (UIPressed[i] == NULL) {
				
					UIPressed[i] = UIOver;
					UIOver->MouseDown(i);
					if (UIOver->CanDrag()) {
						Dragging = UIOver;
					}
					int ct = clock();

					if (ct < (lClick + 250))
					{
						UIOver->DoubleClick(i);
					}

					lClick = ct;

				}

			}
			else {
				if (UIPressed[i] == control) {
					control->MouseUp(i);
					if (UIOver != NULL) {
						if (UIOver != Dragging) {
							if (Dragging != NULL) {
								if (UIOver->CanDrop())
								{
									UIOver->Drop(Dragging);
									UIOver->DropAct(Dragging->GetData());
								}
							}
						}
					}
					Dragging = NULL;
					UIPressed[i] = NULL;
				}
			}
		}
		return true;
	}
	
	else {
		//printf("Out of bounds.\n");
		if (control == UIOver && control!=UIOver) {
			UIOver = NULL;
			control->MouseLeave();
		}

		for (int i = 0; i < 16; i++) {
			if (UIPressed[i] == control) {

				if (But[i] == false) {
					control->MouseUp(i);
					if (UIOver != NULL) {
						if (UIOver != Dragging) {
							if (Dragging != NULL) {
								if (UIOver->CanDrop())
								{
									UIOver->Drop(Dragging);
									UIOver->DropAct(Dragging->GetData());
								}
							}
						}
					}
					Dragging = NULL;
					UIPressed[i]= NULL;
				}

			}
		}
	}
	return false;
};

void UI::DrawTexture(Texture2D* tex, int x, int y, int w, int h, float r, float g, float b, float a,SmartDraw* draw)
{

	draw->DrawTexture(x, y, w, h, tex, r, g, b, a);

}

void UI::DrawTexture(Texture2D* tex, int x, int y, int w, int h, float r, float g, float b, float a) {


	//glClear(GL_DEPTH_BUFFER_BIT);
	//printf("X:%d Y:%d W:%d H:%d \n",x,y,w,h);
//	Drawer->Begin();
	Drawer->DrawTexture(x, y, w, h, tex, r, g, b, a);
	//Drawer->End();
};

void UI::Render() {

	Drawer->Begin();


	RenderControl(UIRoot);

	if (Top != NULL) {

		RenderControl(Top);
	}

	if (Menu != NULL) {

		RenderControl(Menu);

	}
	if (Dragging != NULL) {
		int ox, oy;
		ox = Dragging->GetInternalX();
		oy = Dragging->GetInternalY();
		Dragging->SetOverride(MouseX, MouseY);
		RenderControl(Dragging);
		Dragging->SetOverride(-1, -1);
	}
	
	if (UIOver != NULL) {

		const char* tt = UIOver->GetToolTip();
		if (tt != NULL) {

			int at = clock();

			if (toolTipStart < (at - 1000)) {
				toolTipStart = at + 2000;
			}


			//int at = clock();

			if (at > toolTipStart) {

				int tw = UI::TextWidth(tt) + 10;

				Drawer->DrawTexture(MouseX +10, MouseY - 8, tw, 22, White, 0.8f, 0.8f, 0.8f, 0.9f);
				Drawer->DrawTexture(MouseX + 11, MouseY - 7, tw - 2, 20, White, 0.4f, 0.4f, 0.4f, 1);
				UI::RenderText(MouseX +12, MouseY - 6, tt, 1, 1, 1, 1);
				toolTipStart = at;

			}
		}

	}else{
		toolTipStart = 0;
	}

	if (MainMenu != NULL) {

		RenderControl(MainMenu);

	}
	

	/*
	if (CurrentDrag != NULL) {

		if (UI::But[0] == false) {

			if (UIOver != NULL) {

				UIOver->DropDrag(CurrentDrag);
			}

			CurrentDrag = NULL;

		}
		else {

			Drawer->DrawTexture(MouseX - 10, MouseY - 10, 64, 64, CurrentDrag->Icon, 1, 1, 1, 0.7f);
			UI::RenderText(MouseX - 10, MouseY + 45, CurrentDrag->Text, 1, 1, 1, 1);
		}
	}
	
	for (int i = 0; i < pings.size(); i++) {

		CursorPing ping = pings[i];

		float is = 1.0f;

		ping.r += 3.6f;

		float al = ((float)ping.r) / 32.0f;
		al = 1.0f - al;
				

		if (al < 0) al = 0.0f;

		is = ping.r;
	//	printf("AL:%f\n", al);

	//	Drawer->DrawTex(ping.x - (12 + is), ping.y - (12 + is), 24 + is*2, 24 + is*2,CursorTex, 1, 1, 1, al);

		//al = 1.0f - al;


		if (al < 0.0) al = 0.0f;
		if (al > 1.0) al = 1.0f;

		is = 32.0f - ping.r;

		if (is < 0.0) is = 0.0;

		//Drawer->DrawTex(ping.x - (12 + is), ping.y - (12 + is), 24 + is * 2, 24 + is * 2, CursorTex, 1, 1, 1, al);

		pings[i] = ping;

	}
	*/
	Drawer->DrawTexture(MouseX, MouseY, 26,26, CursorTex, 1, 1, 1, 1.0f);



	Drawer->End();



};

void UI::SetKey(int key,bool state)
{
	UI::Key[key] = state;
	if (UI::Main->GetOver() != NULL) {

		auto act = UI::Main->GetOver();
		if (state) {
			act->KeyDown(key);
		}
		else {
			act->KeyUp(key);
		}

	}

}

void UI::CallKey(int key) {

	if(UI::Main->GetActive()!=NULL)
	{

		auto act = UI::Main->GetActive();

		act->KeyPressed(key,UI::ShiftOn);

	}

}

void UI::RenderControl(UIControl * control) {


	if (control == JoyActive) {

	//	Drawer->DrawTex(control->GetX()-2, control->GetY()-2, control->GetW()+4, control->GetH()+4, White, 0.2f, 1, 1, 1.0);

	}

	

	control->Render();
	int rh = control->GetH();
	int rw = control->GetW();
	for (int i = 0; i < control->ControlsCount(); i++)
	{
	
		auto nc = control->GetControl(i);
		
		int rx = nc->GetInternalX() + nc->GetOffX();
		int ry = nc->GetInternalY() + nc->GetOffY();


		//if (control->GetCullChild()) {
		//	if (rx >= 0 && ry >= 0 && rx <= rw && (ry + nc->GetH()) <= rh) {

				RenderControl(control->GetControl(i));
		//	}
		//}
	//	else {
			RenderControl(control->GetControl(i));
	//	}
	}
}

void UI::SetMouse(int x, int y, int z) {

	UI::LastX = UI::MouseX;
	UI::LastY = UI::MouseY;
	UI::MouseX = x;
	UI::MouseY = y;
	UI::MouseZ = z;

}

void UI::SetMouseBut(int id, bool state) {

	But[id] = state;


};

bool UI::But[];
bool UI::Key[];

int UI::MouseX = 0;
int UI::MouseY = 0;
int UI::MouseZ = 0;
int UI::LastX = 0;
int UI::LastY = 0;




kFont* UI::UIFont = NULL;

void UI::RenderText(int x, int y, const char* txt, float r, float g, float b, float a) {

	//***
//	Drawer->Begin();
	//UIFont->drawText(txt, x, y, r, g, b, a);
//	Drawer->End();
};

void UI::RenderText(int x, int y, const char* txt, float r, float g, float b, float a, IDraw* Draw)
{
	//UIFont->drawText(txt, x, y, r, g, b, a, Draw);
}

int UI::TextWidth(const char* text) {

	
	return 0;
	//return UIFont->getWidth(text);

}

int UI::TextHeight(const char* text) {

	return 0;
	//return UIFont->getHeight();

};

bool UI::ShiftOn = false;