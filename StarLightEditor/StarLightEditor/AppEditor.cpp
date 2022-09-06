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

	Importer* imp = new Importer;

	NodeEntity* n1 = imp->ImportAI("data/test1.fbx", true);

	int b = 5;

	mGraph = new SceneGraph();

	auto real_node =(NodeEntity*) n1->GetChild(0);

	int a = 5;

	mGraph->AddNode(real_node);

	mEnt1 = real_node;

	//mFont1 = new TTFont("data/f1.ttf");
	//mFont1 = new kFont("data/fonts/air.pf");



}

void AppEditor::UpdateApp() {

	mUI->Update();

}
float anX = 0;
float anY = 0;
int dx = 0;
void AppEditor::RenderApp() {

		  std::cout << "Rendering App.\n";
		 mUI->Render();
		 anY++;
		 anX++;
		 mGraph->Render();
		 auto cam = mGraph->GetCamera();
		 cam->SetPosition(float3(0, 0, -15));
		 mEnt1->SetRotation(anX, anY, 0);
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