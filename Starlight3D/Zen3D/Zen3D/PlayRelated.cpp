#include "ZenUI.h"

void ZenUI::BeginPlay() {

	if (!mPlaying) {
		mGraph->BeginPlay();
		mPlaying = true;
	}
}

void ZenUI::EndPlay() {

	if (mPlaying == true) {
		mPlaying = false;
		mGraph->EndPlay();
	}

}