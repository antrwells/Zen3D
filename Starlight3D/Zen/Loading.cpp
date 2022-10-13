#include "ZenUI.h"

bool ZenUI::LoadPayload(FSPayload* pl, void(*done)(FSPayload* pl))
{

	if (mCurrentPayload != nullptr)
	{
		if (mCurrentPayload->LoadedPercent() >= 1.0f)
		{
			mWhenDone(mCurrentPayload);
			mCurrentPayload = pl;
			mCurrentPayload->LoadThreaded();
			mWhenDone = done;
			return true;
		}
		else {
			return false;
		}
	}

	mCurrentPayload = pl;
	mCurrentPayload->LoadThreaded();
	mWhenDone = done;
	return true;

}

float ZenUI::PayloadPercent() {

	if (mCurrentPayload == nullptr) {
		return 1.0f;
	}
	float lp = mCurrentPayload->LoadedPercent();
	if (lp >= 1.0f) {
		mWhenDone(mCurrentPayload);
		mCurrentPayload = nullptr;
	}
	return lp;

}

void ZenUI::LoadingIndicator() {

	if (mCurrentPayload == nullptr) {
		return;

	}



	ImGui::SetNextWindowPos(ImVec2(Application::GetApp()->GetWidth() - 256, Application::GetApp()->GetHeight() - 32));
	ImGui::SetNextWindowSize(ImVec2(256, 32));

	bool open = true;
	ImGui::Begin("Progress",&open, ImGuiWindowFlags_NoDecoration);

	ImGui::ProgressBar(PayloadPercent());

	ImGui::End();

}