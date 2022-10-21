#include "ZenUI.h"
#include "Importer.h"
#include "RayPicker.h"
#include "VString.h"
#include "DirCollection.h"
#include "VString.h"
#include "VFile.h"
bool z = false;

int node_id = 0;

struct NodeRef {

	Node3D* node = nullptr;

};
bool Block = false;

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

	std::string name = std::string(node->GetName());

	if (name.size() < 1)
	{
		name = "NO_NAME";
	}


	if (ImGui::TreeNodeEx(name.c_str(), flag)) {

		if (ImGui::IsItemClicked()) {
			mSelectedNode = node;


			if (ImGui::IsMouseDoubleClicked(0)) {
				mEditNode = node;
			}
		}
		if (ImGui::BeginDragDropSource()) {

			NodeRef* nr = new NodeRef;
			nr->node = mSelectedNode;
			ImGui::SetDragDropPayload("Node", nr, sizeof(NodeRef));
			ImGui::Button(mSelectedNode->GetName(), ImVec2(64, 64));
			ImGui::EndDragDropSource();
			

		}
		else {
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Model"))
				{
					DirEntry* entry = (DirEntry*)payload->Data;
					auto n_node = ImportNode(entry->full.c_str());

					node->AddNode(n_node);

					mDragEntry = nullptr;
					BlockSG = true;

				}
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Node"))
				{
					auto nr = (NodeRef*)payload->Data;
					auto node2 = nr->node;
					//node2->GetRootNode()->RemoveNode(node2);
					node->AddNode(node2);

				}
				ImGui::EndDragDropTarget();
			}
			


			//
		}

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::BeginMenu("Add",true))
			{

				if (ImGui::MenuItem("Light Source"))
				{
					if (mSelectedNode != nullptr) {

						auto new_light = new NodeLight;
						new_light->SetPosition(float3(0, 5, 0));
						mSelectedNode = new_light;
						mGraph->AddLight(new_light);
						int num = mGraph->LightCount();
						VString* lname = new VString("Light ");
						lname->Add(VString(num));
						new_light->SetName(lname->GetConst());

						//	mSelectedNode -
							//exit(1);

					}
				}
				if (ImGui::MenuItem("Camera"))
				{
					auto new_cam = new NodeCamera;
					mGraph->AddCamera(new_cam);
					//mGameCam = new_cam;

				}
				ImGui::EndMenu();
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

																											
	}

	

	ImGui::EndChild();


	ImGui::End();

}