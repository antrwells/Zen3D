#include "UITest.h"
#include "Texture2D.h"
#include "UIImage.h"
#include "UIButton.h"
#include "UIWindow.h"
#include "WindowPage.h"
void UITest::InitApp()
{
	mUI = new UI;

	UIBase* test = new UIBase;

	//test->SetImage(new Texture2D("UI/Theme/Bright/FrameSquare.png"));
	auto img1 = new UIImage;
	img1->SetRect(UIRect(0, 0,Application::GetApp()->GetWidth(),Application::GetApp()->GetHeight()));
	mUI->Add(img1);



	auto win = new UIWindow("Viewport");
	win->SetRect(UIRect(200, 200,400,500));
	img1->Add(win);

	auto page = win->GetPage();

	auto but2 = new UIButton;
	but2->SetRect(UIRect(64, 64, 128, 32));
	//img1->Add(but1);
	but2->SetText("Moving");
	page.content->Add(but2);
	auto settings = win->AddPage("Settings");

	auto but3 = new UIButton;
	but3->SetText("Test");
	but3->SetRect(UIRect(8, 8, 128, 30));
	settings.content->Add(but3);





//	mUI->Add(img1);

}

void UITest::UpdateApp() {

	mUI->Update();

}

void UITest::RenderApp() {

	mUI->Render();

}