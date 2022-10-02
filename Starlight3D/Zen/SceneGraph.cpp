#include "ZenUI.h"
#include "Importer.h"
#include "RayPicker.h"
#include "VString.h"
#include "DirCollection.h"
#include "VString.h"
#include "VFile.h"
bool z = false;

int node_id = 0;

void ZenUI::SceneTree(Node3D* node)
{

	ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;



	if (node == mSelectedNode) {

		//flags = ImGuiTreeNodeFlags_Selected;
	//	printf("S>>>");
	//	printf(node->GetName());
		//printf("\n");

	}

	if (node->ChildrenCount() == 0) {

		flag |= ImGuiTreeNodeFlags_Leaf;

	}

	if (node == mSelectedNode)
	{
		flag |= ImGuiTreeNodeFlags_Selected;
	}

	if (ImGui::TreeNodeEx(node->GetName(), flag)) {


		if (ImGui::IsItemClicked()) {
			mSelectedNode = node;
		}


		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Add Script"))
			{

				if (mSelectedNode != nullptr) {



					//	mSelectedNode -
						//exit(1);

				}

			}

			ImGui::EndPopup();
		}
		if (node->ChildrenCount() > 0) {

			for (int i = 0; i < node->ChildrenCount(); i++) {

				node_id++;
				SceneTree(node->GetChild(i));

			}
		}


		//printf("Selected Node!");

		ImGui::TreePop();
	}
	else {



	}


}

void ZenUI::SceneGraphWindow() {


	if (!mSGF) {


		ImGui::SetNextWindowPos(mSceneGraphPos);
		ImGui::SetNextWindowSize(mSceneGraphSize);
		mSGF = true;
	}

	ImGui::Begin("Scene Graph", &mSceneGraphOpen, ImGuiWindowFlags_MenuBar);
	ImGui::BeginChild("SceneTree");

	node_id = 0;
	SceneTree(mGraph->GetRoot());
	if (mSelectedNode != nullptr) {

		printf("Selected Node:");
		printf(mSelectedNode->GetName());
		printf("\n");																										
	}

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Model"))
		{
			DirEntry* entry = (DirEntry*)payload->Data;
			ImportNode(entry->full.c_str());
			mDragEntry = nullptr;

		}
		ImGui::EndDragDropTarget();
	}

	ImGui::EndChild();


	ImGui::End();

}