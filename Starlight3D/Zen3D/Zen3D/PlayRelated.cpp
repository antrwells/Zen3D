#include "ZenUI.h"

void ZenUI::BeginPlay() {

	if (!mPlaying) {
	//	if (mCurrentCamera == mEditCam) {
		//	mCurrentCamera = mGameCam;
	//		mGraph->SetCamera(mGameCam);
	//	}
		mGraph->BeginPlay();
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
		mGraph->EndPlay();
	}

}