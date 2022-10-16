#include "UITestApp.h"


void UITestApp::InitApp()
{
	mDraw = new SmartDraw(Application::GetApp());
	tex1 = new Texture2D("edit/splash1.png");
}


void UITestApp::UpdateApp() {

}

void UITestApp::RenderApp() {


	mDraw->Begin();

	mDraw->DrawTexture(20, 20, 256, 256, tex1, 1, 1, 1, 1, false);

	mDraw->End();

}