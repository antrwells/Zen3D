#include "ZenUI.h"


void ZenUI::ImportModelWindow() {

	if (mImportModelFirst) {

		ImGui::SetNextWindowPos(ImVec2(120, 120));
		ImGui::SetNextWindowSize(ImVec2(400, 500));
		mImportModelFirst = false;
	}

	ImGui::Begin("Import Model", &mImportModelOpen);
	std::string importText = "Importing Mode:" + mImportModelPath;
	ImGui::Text(importText.c_str());
	ImGui::Checkbox("Actor", &mImportActor);
	if (ImGui::Button("Import")) {

		if (mImportActor) {
			auto node = ImportActor(mImportModelPath.c_str());
			mGraph->AddNode((Node3D*)node);
			node->SetFilePath(mImportModelPath.c_str());

		}
		else {
			auto node = ImportNode(mImportModelPath.c_str());
			mGraph->AddNode(node);
			node->SetFilePath(mImportModelPath.c_str());
		}
	}

	ImGui::End();

}