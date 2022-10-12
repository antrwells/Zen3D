#include "ZenUI.h"

void ZenUI::PostProcessWindow() {

	if (!mPostProcessOpen) {
		return;
	}

	if (mPPFirst) {

		ImGui::SetNextWindowPos(ImVec2(256, 156));
		ImGui::SetNextWindowSize(ImVec2(512, 400));
		mPPFirst = false;
	}

	ImGui::Begin("Post Processing");

	ImGui::BeginChild(128);
	
	//General
	if (ImGui::Checkbox("Post Processing?", &mPPOn))
	{

	}

	//Bloom

	float cl = mPPBloom->GetLimit();
	float bl = mPPBloom->GetBlur();

	ImGui::Text("Bloom");

	ImGui::DragFloat("Color Threshold", &cl, 0.01f, 0.01f, 1.0f);

	ImGui::DragFloat("Blur", &bl,0.00005f,0.00001f,1.5f);

	mPPBloom->Set(cl, bl);


	ImGui::EndChild();
	ImGui::End();

}