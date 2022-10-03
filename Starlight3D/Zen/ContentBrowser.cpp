#include "ZenUI.h"
#include "Importer.h"
#include "RayPicker.h"
#include "VString.h"
#include "DirCollection.h"
#include "VString.h"
#include "VFile.h"
// Content Browser

void ZenUI::MainContentBrowser() {

	if (!mCBF) {


		ImGui::SetNextWindowPos(mContentBrowserPos);
		ImGui::SetNextWindowSize(mContentBrowserSize);
		mCBF = true;
	}

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
				if (item_name.Len() > 8)
				{
					item_name = item_name.SubString(0, 8);
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
						auto nn = ImportNode(entry.full.c_str());
						mGraph->AddNode(nn);
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
						else if (mDragEntryRef.ext == "l") {
							ImGui::SetDragDropPayload("Script", mDragEntry, sizeof(DirEntry), ImGuiCond_Once);
						}
						ImGui::Button(mDragEntryRef.name.c_str(), ImVec2(64, 64));
						ImGui::EndDragDropSource();
						
					}
				
				ImGui::SetCursorPos(ImVec2(cx + 5, cy + 62));
				VString item_name = VString(entry.name.c_str());
				if (item_name.Len() > 8)
				{
					item_name = item_name.SubString(0, 8);
				}
				ImGui::Text(item_name.GetConst());
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

		//ImGui::SetCursorPos(ImVec2(128, 128));
		//ImGui::Button("Folder");
		//ImGui::SetCursorPos(ImVec2(5, 64));
		//ImGui::Text("Contents");



	}

	ImGui::End();

}
