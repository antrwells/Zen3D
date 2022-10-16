#include "ZenUI.h"

void ZenUI::LightMapperWindow() {

	if (!mLightmapperOpen) {
		return;
	}

	if (mLMFirst) {

		ImGui::SetNextWindowPos(ImVec2(256, 156));
		ImGui::SetNextWindowSize(ImVec2(512, 400));
		mLMFirst = false;
	}

	ImGui::Begin("Light mapper");

	ImGui::BeginChild(128);

	int res[2];
	res[0] = mLMRes[0];
	res[1] = mLMRes[1];
	if (ImGui::DragInt2("Resolution", res, 32, 32, 4096 * 2))
	{
		mLMRes[0] = res[0];
		mLMRes[1] = res[1];
	}

	if (ImGui::Button("Render"))
	{

	}

	ImGui::EndChild();

	ImGui::End();

}