#include "ZNotification.h"
#include "Application.h"
ZNotification::ZNotification(std::string title, std::string msg) {

	mTitle = title;
	mMsg = msg;
	mX = Application::GetApp()->GetWidth() - 300;
	mY = Application::GetApp()->GetHeight() + 40;

}

int wait_time = 0;

bool ZNotification::Update() {

	if (mUp) {

		if (mY > Application::GetApp()->GetHeight() - 150) {

			mY = mY - 5;

		}
		else {
			mWait = true;
			mUp = false;
			wait_time = clock() + 3500;
		}
	}

	if (mWait) {

		if (clock() > wait_time) {
			mWait = false;
			mDown = true;
		}

	}

	if (mDown) {

		mY = mY + 5;
		if(mY>Application::GetApp()->GetHeight()+80)
		{
			mDone = true;
			return true;
		}

	}
	return false;

}