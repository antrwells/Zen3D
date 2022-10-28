#include "ZenUI.h"
#include <memory>
#include "Importer.h"
#include "RayPicker.h"

void ZenUI::SaveNodeDialog() {

	ImGui::SetNextWindowPos(ImVec2(Application::GetApp()->GetWidth() / 2 - 180, Application::GetApp()->GetHeight() / 2 - 40));
	ImGui::SetNextWindowSize(ImVec2(360, 80));

	ImGui::Begin("Save Node As...", &mSaveSceneOpen, ImGuiWindowFlags_NoDecoration);

	if (saveName == nullptr) {
		saveName = (char*)malloc(2049);
		saveName[0] = "\0"[0];
	}

	if (ImGui::InputText("Filename", (char*)saveName, 2048)) {

		//mSaveSceneOpen = false;


	}

	if (ImGui::Button("Save"))
	{
		SaveNode((const char*)saveName);

		mSaveNodeOpen = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel"))
	{
		mSaveNodeOpen = false;
	}

	ImGui::End();


}

void ZenUI::SaveSceneDialog() {

	ImGui::SetNextWindowPos(ImVec2(Application::GetApp()->GetWidth() / 2 - 180, Application::GetApp()->GetHeight() / 2 - 40));
	ImGui::SetNextWindowSize(ImVec2(360, 80));

	ImGui::Begin("Save Current Scene As...",&mSaveSceneOpen ,ImGuiWindowFlags_NoDecoration);

	if (saveName == nullptr) {
		saveName = (char*)malloc(2049);
		saveName[0] = "\0"[0];
	}

	if (ImGui::InputText("Filename", (char*)saveName, 2048)) {

		//mSaveSceneOpen = false;


	}

	if (ImGui::Button("Save"))
	{
		SaveScene((const char*)saveName);
		
		mSaveSceneOpen = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel"))
	{
		mSaveSceneOpen = false;
	}

	ImGui::End();

}

void ZenUI::LoadScene(const char* path) {

	mGraph->LoadGraph(path);
	mCurrentCamera = mGraph->GetCamera();
	mEditCam = mCurrentCamera;

	auto cam_rot = mGraph->GetProperty("CameraRotation");
	cam_rotation.x = cam_rot->GetFloat2().x;
	cam_rotation.y = cam_rot->GetFloat2().y;
	
	ZenUI::mUI->Notify("Imported scene.", "Scene imported succesfully.");

}
void ZenUI::LoadNode(const char* path) {

	VFile* file = new VFile(path, FileMode::Read);

	auto node = mGraph->ReadNodeHeader(file);
	mGraph->ReadNode(file, node);

	file->Close();
	mGraph->AddNode(node);
	ZenUI::mUI->Notify("Imported Node.", "Succesfully imported node.");

}
void ZenUI::SaveNode(const char* path) {
	std::string fpath(mContentPath->GetConst());
	fpath = fpath + "/" + std::string(path);
	fpath = fpath + ".znode";

	auto node = mSelectedNode;

	if (node->GetType() == NodeType::Entity)
	{
		NodeEntity* entity = (NodeEntity*)node;

		VFile* file = new VFile(fpath.c_str(), FileMode::Write);

		//mGraph->WriteTransform(file, node);
		mGraph->SaveNodeHeader(file, node);
		mGraph->SaveNode(file, node);

		file->Close();

	}

}

void ZenUI::SaveScene(const char* path)
{
	std::string fpath(mContentPath->GetConst());
	fpath = fpath + "/" + std::string(path);
	fpath = fpath + ".zscene";

	NodeProperty* cam_rot = new NodeProperty("CameraRotation");
	cam_rot->SetFloat2(float2(cam_rotation.x, cam_rotation.y));
	mGraph->AddProperty(cam_rot);

	mGraph->SaveGraph(fpath);


	/*
	std::string fpath(mContentPath->GetConst());
	fpath = fpath + "/"+ std::string(path);
	fpath = fpath + ".zscene";

	VFile* file_out = new VFile(fpath.c_str(), FileMode::Write);

	std::vector<std::string> lroot;
	file_out->WriteInt(mGraph->LightCount());
	for (int i = 0; i < mGraph->LightCount(); i++)
	{
		auto light = mGraph->GetLight(i);
		light->WriteTransform(file_out);
		file_out->WriteVec3(light->GetDiffuse());
		file_out->WriteVec3(light->GetSpecular());
		file_out->WriteFloat(light->GetRange());
		file_out->WriteString(light->GetName());
		file_out->WriteVec3(light->GetCone());
		file_out->WriteInt((int)light->GetLightType());
		file_out->WriteBool(light->GetCastShadows());
		file_out->WriteBool(light->GetEnabled());
		
	
	}



	auto node1 = mGraph->GetRoot();

	node1->WriteNode(file_out);

	file_out->WriteString(mCurrentCamera->GetName());

	//Write_Node(file_out, node1);


	file_out->Close();
	
	

	//Save file info file for progress bar.
	*/
	
	ZenUI::mUI->Notify("Scene Saved", "Succesfully saved scene " + std::string(fpath));

}