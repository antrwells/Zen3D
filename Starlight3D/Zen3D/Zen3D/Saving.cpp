#include "ZenUI.h"
#include <memory>
#include "Importer.h"
#include "RayPicker.h"
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

void Read_Node(VFile* file, Node3D* node) {

	bool just_node = file->ReadBool();
	if (just_node == false) {
		node->ReadTransform(file);
		node->SetName(VString(file->ReadString()).GetConst());
		int nc = file->ReadInt();
		for (int i = 0; i < nc; i++) {
			auto new_node = new Node3D;
			Read_Node(file, new_node);
			node->AddNode(new_node);
		}
	
	}
	else {

		std::string path(file->ReadString());
		auto imp = new Importer;
		auto im = imp->ImportAI(path.c_str());
		//im->ReadTransform(file);
		node->ReadTransform(file);
		im->SetName(VString(file->ReadString()).GetConst());
		node->AddNode(im);
		node->SetName(im->GetName());


	}

}

void Write_Node(VFile* file, Node3D* node)
{

	std::string n_path = node->GetFilePath();

	if(n_path == "None")
	{
		file->WriteBool(false);
		node->WriteTransform(file);
		file->WriteString(node->GetName());

	}
	else {

		file->WriteBool(true);
		file->WriteString(n_path.c_str());
		node->WriteTransform(file);
		file->WriteString(node->GetName());
	}

	file->WriteInt(node->ChildrenCount());
	for (int i = 0; i < node->ChildrenCount();i++)
	{

		Write_Node(file, node->GetChild(i));

	}


}

void ZenUI::LoadScene(const char* path) {

	VFile* file = new VFile(path, FileMode::Read);

	mGraph->Reset();

	NodeEntity* root = new NodeEntity();

	int lc = file->ReadInt();
	for (int i = 0; i < lc; i++) {
		auto nl = new NodeLight;
		nl->ReadTransform(file);
		nl->SetDiffuse(file->ReadVec3());
		nl->SetSpecular(file->ReadVec3());
		nl->SetRange(file->ReadFloat());
		nl->SetName(file->ReadString());
		nl->SetCone(file->ReadVec3());
		nl->SetLightType((LightType)file->ReadInt());
		mGraph->AddLight(nl);
		root->AddNode(nl);

	}
	
	root->ReadNode(file,true);

	mGraph->SetRoot(root);

	mRayPick->SetGraph(mGraph);

	file->Close();

	ZenUI::mUI->Notify("Imported scene.", "Scene imported succesfully.");

}

void ZenUI::SaveScene(const char* path)
{

	std::string fpath(mContentPath->GetConst());
	fpath = fpath + "/"+ std::string(path);
	fpath = fpath + ".zscene";

	VFile* file_out = new VFile(fpath.c_str(), FileMode::Write);

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
		
		auto getRoot = light->GetRootNode();
		getRoot->RemoveNode(light);

	}



	auto node1 = mGraph->GetRoot();

	node1->WriteNode(file_out);

	//Write_Node(file_out, node1);


	file_out->Close();
	
	//Save file info file for progress bar.

	
	ZenUI::mUI->Notify("Scene Saved", "Succesfully saved scene " + std::string(fpath));

}