#include "SLApp.h"

void SLApp::InitApp() {

	mDraw = new SmartDraw(this);
	mTex1 = new Texture2D("data/test1.jpg");
	mTex2 = new Texture2D("data/test2.jpg");
	mTex3 = new Texture2D("data/test3.jpg");

}

void SLApp::UpdateApp() {

	std::cout << "Updating app.\n";

}

int dx = 0;
void SLApp::RenderApp() {

	std::cout << "Rendering App.\n";

	mDraw->Begin();

	

	mDraw->DrawTexture(dx+20, dx+20, 290, 200,mTex1, 1, 0, 0, 1);
	mDraw->DrawTexture(dx+200, 200, 255, 255,mTex2, 0, 1, 0, 1);
	mDraw->DrawTexture(dx+100, 600, 96, 96,mTex3, 0, 0, 1,1);


	dx = dx + 16;
	if (dx > 1024) {
		dx = 0;
	}

	mDraw->End();

	dx = dx + 1;
}
