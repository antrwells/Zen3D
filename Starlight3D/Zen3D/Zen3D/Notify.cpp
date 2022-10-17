#include "ZenUI.h"


void ZenUI::Notify(std::string title, std::string msg) {

	ZNotification* notify = new ZNotification(title, msg);
	mNotices.push_back(notify);

}

void ZenUI::Notifications() {

	if (mNotices.size() == 0 && mCurrentNotice == nullptr) {
		return;
	}

	if (mCurrentNotice == nullptr) {

		if (mNotices.size() > 0) {
			mCurrentNotice = mNotices[0];
		}
		

	}

	ImGui::SetNextWindowPos(ImVec2(mCurrentNotice->GetX(), mCurrentNotice->GetY()));
	ImGui::SetNextWindowSize(ImVec2(280, 120));
	ImGui::SetNextWindowBgAlpha(0.75f);
	mCurrentNotice->Update();

	int flags = ImGuiWindowFlags_NoDecoration;

	bool open = true;
	ImGui::Begin(mCurrentNotice->GetTitle().c_str(),&open,flags);
	ImGui::Text(mCurrentNotice->GetTitle().c_str());
	ImGui::TextWrapped(mCurrentNotice->GetMsg().c_str());

	ImGui::End();

	if (mCurrentNotice->Done()) {

		mNotices.erase(mNotices.begin());
		mCurrentNotice = nullptr;
	}

}