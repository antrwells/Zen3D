#include "TestVideoApp.h"
#include "VideoDecoder.h"
#include "SmartDraw.h"

void TestVideoApp::InitApp() {
	mDraw = new SmartDraw(Application::GetApp());
	//mTex1 = new Texture2D("UI/ButtonFrame.png");
	mDecoder = new VideoDecoder;



	bool res =  mDecoder->OpenVideo("video/intro2.mov");

	if (!res) {
		printf("Failed to open video.\n");
		exit(0);
	}
	else {
		printf("Opened video succesfully.");
	}
}

void TestVideoApp::UpdateApp() {
	mDecoder->GetCurrentFrame();
	mTex1 = mDecoder->GetCurrentImage();
}


void TestVideoApp::RenderApp() {

	mDraw->Begin();

	if (mTex1 != nullptr) {
		mDraw->DrawTexture(0, 0, Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight(), mTex1, 1, 1, 1, 1);
	}
	mDraw->End();

}