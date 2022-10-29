#include "ZenUI.h"

void ZenUI::BeginPlay() {

	if (!mPlaying) {
	//	if (mCurrentCamera == mEditCam) {
		//	mCurrentCamera = mGameCam;
	//		mGraph->SetCamera(mGameCam);
	//	}
		mGameGraph = new SceneGraph;
		mGameGraph->SetRoot(mGraph->GetRoot());
		mGameGraph->SetMain();
		mGameGraph->BeginPlay();
		//mGameGraph->SetRoot(new Node3D);

		//mGraph->BeginPlay();
		
		mPlaying = true;
	}
}

void ZenUI::EndPlay() {

	if (mPlaying == true) {
		//if (mCurrentCamera == mGameCam) {
		//	mCurrentCamera = mEditCam;
	//		mGraph->SetCamera(mEditCam);
	//	}
	
		mPlaying = false;
		mGameGraph->EndPlay();
		mGameGraph = nullptr;
	}

}