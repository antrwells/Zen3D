#include "AppEditor.h"
#include "UITheme_Neon.h"
#include "ButtonControl.h"

AppEditor::AppEditor() {



}


void AppEditor::InitApp() {
	mDraw = new SmartDraw(this);
	mTex1 = new Texture2D("data/test1.jpg");
	mTex2 = new Texture2D("data/test2.jpg");
	mTex3 = new Texture2D("data/test3.jpg");
	mUI = new UI(0, 0);
	mUI->SetInput(GetInput());
	UI::SetTheme(new UITheme_Neon);

	ButtonControl* but1 = new ButtonControl;
	but1->Set(20, 20, 200, 100);
	but1->SetText("Testing!");
	mUI->GetRoot()->AddControl(but1);

	//mFont1 = new TTFont("data/f1.ttf");
	//mFont1 = new kFont("data/fonts/air.pf");



}

void AppEditor::UpdateApp() {

	mUI->Update();

}

int dx = 0;
void AppEditor::RenderApp() {

		  std::cout << "Rendering App.\n";
		 mUI->Render();
		 // mFont1->drawText("aaabbbcccddddeeefffggghhhiiijjjklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 20, 300, 1, 1, 1, 1,mDraw);
		 // mFont1->drawText("This is another test to see if text rendering is working 0123456789", 20, 400, 1, 1, 1, 1, mDraw);
		  /*
	mDraw->Begin();

	mFont

	


	mDraw->DrawTexture(dx+20, dx+20, 290, 200,mTex1, 1, 0, 0, 1);
	mDraw->DrawTexture(dx+200, 200, 255, 255,mTex2, 0, 1, 0, 1);
	mDraw->DrawTexture(dx+100, 600, 96, 96,mTex3, 0, 0, 1,1);


	dx = dx + 1;
	if (dx > 1024) {
		dx = 0;
	}

	mDraw->End();
	*/
	dx = dx + 1;

}