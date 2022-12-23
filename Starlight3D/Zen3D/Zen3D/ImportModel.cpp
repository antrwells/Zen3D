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

	float scal[3];

	scal[0] = 1;
	scal[1] = 1;
	scal[2] = 1;

	float rot[3];

	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;

	bool is_act = false;

	if (ImGui::DragFloat3("Import Scale", scal, 0.001f, 0.00001f, 2048))
	{
	
	}

	ImGui::Checkbox("Actor", &mImportActor);
	if (ImGui::Button("Import")) {

		if (mImportActor) {
			auto node = ImportActor(mImportModelPath.c_str());
			mGraph->AddNode((Node3D*)node);
			node->SetFilePath(mImportModelPath.c_str());
			node->SetScale(float3(scal[0], scal[1], scal[2]));
			mImportModelOpen = false;

			std::string ip = mImportModelPath + ".inf";

			VFile* inf = new VFile(ip.c_str(), FileMode::Write);
			inf->WriteVec3(float3(scal[0], scal[1], scal[2]));
			inf->WriteVec3(float3(rot[0], rot[1], rot[2]));
			inf->WriteBool(true);
			inf->Close();

		}
		else {
			auto node = ImportNode(mImportModelPath.c_str());
			mGraph->AddNode(node);
			node->SetFilePath(mImportModelPath.c_str());
			node->SetScale(float3(scal[0], scal[1], scal[2]));
			mImportModelOpen = false;
			std::string ip = mImportModelPath + ".inf";
			VFile* inf = new VFile(ip.c_str(), FileMode::Write);
			inf->WriteVec3(float3(scal[0], scal[1], scal[2]));
			inf->WriteVec3(float3(rot[0], rot[1], rot[2]));
			inf->WriteBool(false);
			inf->Close();
		}
	}

	ImGui::End();

}