#include "ZenUI.h"
#include "Importer.h"
void ZenUI::MainMenu() {
	//return;

	if (fileBrowse.HasSelected())
	{

		std::string pick = fileBrowse.GetSelected().string();
		LoadScene(pick.c_str());
		fileBrowse.ClearSelected();
		fileBrowseOpen = false;
	}

	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("Projects"))
		{

			if (ImGui::MenuItem("Load Project")) {

			}
			if (ImGui::MenuItem("Save Project"))
			{

			}

			ImGui::MenuItem("-----------------");
			if (ImGui::MenuItem("Load Scene"))
			{
				fileBrowse.Open();
				fileBrowseOpen = true;
			}
			if (ImGui::MenuItem("Save Scene"))
			{
				OpenSaveScene();
			}
			ImGui::MenuItem("-----------------");
			if (ImGui::MenuItem("Exit Zen"))
			{
				exit(1);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Delete"))
			{
				if (mSelectedNode != nullptr) {
					auto root = mSelectedNode->GetRootNode();
					root->RemoveNode(mSelectedNode);
					//mSelectedNode = nullptr;

					if (mSelectedNode->GetType() == NodeType::Light) {
						mGraph->RemoveLight(mSelectedNode);
					}
					mSelectedNode = nullptr;
				}
			}
			if (ImGui::MenuItem("Copy"))
			{
				mClipNode = mSelectedNode;
			}
			if (mClipNode != nullptr) {
				if (ImGui::MenuItem("Paste"))
				{
					auto new_node = mClipNode->Clone();
					mGraph->AddNode(new_node);
					new_node->SetPosition(float3(0, 5, 0));
					new_node->SetRotation(mClipNode->GetRotation4x4());
					mSelectedNode = new_node;

				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Renderer"))
		{
			if (ImGui::MenuItem("Post Processing"))
			{
				mPostProcessOpen = true;
			}
			if (ImGui::MenuItem("Lightmapper"))
			{
				mLightmapperOpen = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Node"))
		{
			if (ImGui::MenuItem("Save Node"))
			{
				OpenSaveNode();
			}
		
			if (ImGui::BeginMenu("Add Primitive"))
			{
				if (ImGui::MenuItem("Plane"))
				{
					Importer* imp = new Importer;
					auto box = imp->ImportAI("data/primitive/plane.fbx");
					mGraph->AddNode(box);
				}
				if (ImGui::MenuItem("Box"))
				{
					Importer* imp = new Importer;
					auto box = imp->ImportAI("data/primitive/cube.fbx");
					mGraph->AddNode(box);


				}
				if (ImGui::MenuItem("Sphere"))
				{
					Importer* imp = new Importer;
					auto box = imp->ImportAI("data/primitive/sphere.fbx");
					mGraph->AddNode(box);
				}
				if (ImGui::MenuItem("Cylinder"))
				{
					Importer* imp = new Importer;
					auto box = imp->ImportAI("data/primitive/cylinder.fbx");
					mGraph->AddNode(box);
				}
				if (ImGui::MenuItem("Cone"))
				{
					Importer* imp = new Importer;
					auto box = imp->ImportAI("data/primitive/cone.fbx");
					mGraph->AddNode(box);
				}
				if (ImGui::MenuItem("Torus"))
				{
					Importer* imp = new Importer;
					auto box = imp->ImportAI("data/primitive/torus.fbx");
					mGraph->AddNode(box);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}


}