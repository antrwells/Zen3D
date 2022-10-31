#include "ZenUI.h"
#include "Importer.h"
#include "RayPicker.h"
#include "VString.h"
#include "DirCollection.h"
#include "VString.h"
#include "VFile.h"
// Content Browser
struct NodeRef {

	Node3D* node = nullptr;

};
void import_modeldone(FSPayload* pl) {

	//if(pl->GetName())
	auto model = (NodeEntity*)pl->GetResource("Import");
	
	auto ares = pl->GetResourceObj("Import");

	model->SetFilePath(ares->GetPath());

	ZenUI::mUI->GetGraph()->AddNode(model);


	ZenUI::mUI->Notify("Model imported.", "Imported model succesfully:"+ares->GetPath());

}

void ZenUI::MainContentBrowser() {

	if (!mCBF) {


		ImGui::SetNextWindowPos(mContentBrowserPos);
		ImGui::SetNextWindowSize(mContentBrowserSize);
		mCBF = true;
		
	}
	ImGuiIO& io = ImGui::GetIO();


	int flags = ImGuiWindowFlags_MenuBar;

	int cx, cy;
	cx = 2;
	cy = 2;

	if (ImGui::Begin("Content Browser", &mContentBrowserOpen, flags))
	{
		int id = 0;
		ImGui::BeginChild("Content");

		


		for (int i = 0; i < mDir->enteries.size(); i++) {

			auto entry = mDir->enteries[i];
			if (entry.folder)
			{
				ImGui::SetCursorPos(ImVec2(cx, cy));

				ImGui::PushID(id);
				if (ImGui::ImageButton(mIconFolder->GetView(), ImVec2(58, 58))) {

					int aa = 5;

					ScanContent(entry.full);

					if (ImGui::IsMouseDoubleClicked(ImGuiButtonFlags_MouseButtonLeft)) {
						int b = 5;
					}

				}
				ImGui::SetCursorPos(ImVec2(cx + 5, cy + 62));
				ImGui::Text(entry.name.c_str());
				cx = cx + 120;
				VString item_name = VString(entry.name.c_str());
				if (item_name.Len() > 14)
				{
					item_name = item_name.SubString(0, 14);
				}
				ImGui::Text(item_name.GetConst());
				ImGui::PopID();
				id++;
				if (cx > ImGui::GetWindowSize().x - 80)
				{
					cx = 2;
					cy = cy + 80;
				}

			}
		}

		for (int i = 0; i < mDir->enteries.size(); i++) {

			auto entry = mDir->enteries[i];
			if (!entry.folder)
			{
				ImGui::SetCursorPos(ImVec2(cx, cy));
				ImGui::PushID(id);

				Texture2D* mIcon = mIconFile;

				if (entry.ext == "fbx" || entry.ext == "dae")
				{
					mIcon = mIconFile;
				}
				if (entry.ext == "ch")
				{
					mIcon = mIconScript;
				}

				if (ImGui::ImageButton(mIcon->GetView(), ImVec2(58, 58)))
				{


					//int ab = 5;
					int ab = 5;

					if (entry.ext == "fbx" || entry.ext == "dae")
					{
						//FSPayload* pl = new FSPayload("Load model");
						//pl->AddFile("Import", entry.full.c_str(), ResourceType::ModelProp);
						//LoadPayload(pl, import_modeldone);
						//auto nn = ImportNode(entry.full.c_str());
						ImportModel(entry.full.c_str());
						//mGraph->AddNode(nn);
					}
					if (entry.ext == "zscene")
					{

						LoadScene(entry.full.c_str());

					}
					if (entry.ext == "znode")
					{
						LoadNode(entry.full.c_str());
					}
					if (entry.ext == "ch")
					{




					}
				}

					if (ImGui::BeginDragDropSource()) {

						if (mDragEntry == nullptr) {
							mDragEntryRef = entry;
							mDragEntry = &mDragEntryRef;
						}

						if (mDragEntryRef.ext == "fbx")
						{
							ImGui::SetDragDropPayload("Model", mDragEntry, sizeof(DirEntry), ImGuiCond_Once);
						}
						else if (mDragEntryRef.ext == "zs") {
							ImGui::SetDragDropPayload("Script", mDragEntry, sizeof(DirEntry), ImGuiCond_Once);
						}
						else if (mDragEntryRef.ext == "png" || mDragEntryRef.ext=="jpg")
						{
							ImGui::SetDragDropPayload("Texture", mDragEntry, sizeof(DirEntry), ImGuiCond_Once);
						}
						else if (mDragEntryRef.ext == "mp3" || mDragEntryRef.ext == "wav" || mDragEntryRef.ext == "ogg") {
							ImGui::SetDragDropPayload("Sound", mDragEntry, sizeof(DirEntry), ImGuiCond_Once);
						}
						else if (mDragEntryRef.ext == "zscene") {
							ImGui::SetDragDropPayload("Scene", mDragEntry, sizeof(DirEntry), ImGuiCond_Once);
						}
						else if (mDragEntryRef.ext == "mov")
						{
							ImGui::SetDragDropPayload("Movie", mDragEntry, sizeof(DirEntry), ImGuiCond_Once);
						}
						ImGui::Button(mDragEntryRef.name.c_str(), ImVec2(64, 64));
						ImGui::EndDragDropSource();
						
					}
				
				ImGui::SetCursorPos(ImVec2(cx + 5, cy + 62));
				VString item_name = VString(entry.name.c_str());
				if (item_name.Len() > 14)
				{
					item_name = item_name.SubString(0, 14);
				}

				
				ImGui::Text(item_name.GetConst());
				ImGui::SetCursorPos(ImVec2(cx -28, cy + 6));
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
				//ImGui::InputText("##text1", txt_green, sizeof(txt_green));

				

				//ImGui::PushFont(bigFont);

				ImGui::Text(entry.ext.c_str());
				//ImGui::PopFont();
				ImGui::PopStyleColor();
				ImGui::PopID();
				id++;
				cx = cx + 120;
				if (cx > ImGui::GetWindowSize().x - 80)
				{
					cx = 2;
					cy = cy + 80;
				}

			}
		}
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("New Node Script"))
			{

				mGetScriptName = true;

			}

			ImGui::EndPopup();
		}
		ImGui::EndChild();
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Node"))
			{
				auto nr = (NodeRef*)payload->Data;
				auto node2 = nr->node;
				mSelectedNode = node2;
				SaveNode(node2->GetName());
				ScanContent(mContentPath->GetConst());
				//node2->GetRootNode()->RemoveNode(node2);
				//node->AddNode(node2);


			}
			ImGui::EndDragDropTarget();
		} 
		//ImGui::SetCursorPos(ImVec2(128, 128));
		//ImGui::Button("Folder");
		//ImGui::SetCursorPos(ImVec2(5, 64));
		//ImGui::Text("Contents");



	}

	ImGui::End();

}
