#include "ZenUI.h"

void ZenUI::MainMenu() {
	//return;

	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("Projects"))
		{

			if (ImGui::MenuItem("Load Project")) {

			}
			if (ImGui::MenuItem("Save Project"))
			{

			}
			if (ImGui::MenuItem("Save Scene"))
			{
				OpenSaveScene();
			}

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

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


}