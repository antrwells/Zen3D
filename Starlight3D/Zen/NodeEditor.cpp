#include "ZenUI.h"
#include "Importer.h"
#include "RayPicker.h"
#include "VString.h"
#include "DirCollection.h"
#include "VString.h"
#include "VFile.h"
#include <malloc.h>
#include "ScriptObject.h"
#include "NodeProperty.h"
#include <malloc.h>

void ZenUI::MainNodeEditor() {

	if (!mNEF)
	{
		ImGui::SetNextWindowPos(mNodeEditPos);
		ImGui::SetNextWindowSize(mNodeEditSize);
		mNEF = true;
	}

	int flags = ImGuiWindowFlags_MenuBar;

	if (ImGui::Begin("Node Editor"))
	{

		if (mSelectedNode != nullptr) {
			//ImGui::Text(mSelectedNode->GetName());

			ImGui::BeginChild(55);
			
			char* buf = (char*) malloc(512);
			auto name = mSelectedNode->GetName();
			
			for (int i = 0; i < 512; i++) {

				int ch = name[i];
				if (ch == "\0"[0])
				{
					buf[i] = "\0"[0];
					break;
				}
				buf[i] = ch;

			}

			if (ImGui::InputText("Name", buf, 512)) {

				mSelectedNode->SetName(buf);

			}
			else {
				free(buf);
			}

			
			auto pos = mSelectedNode->GetPosition();
			float posf[3];
			posf[0] = pos.x;
			posf[1] = pos.y;
			posf[2] = pos.z;

			if (ImGui::DragFloat3("Position", posf,0.1f)) {
				mSelectedNode->SetPosition(float3(posf[0], posf[1], posf[2]));
			}


			auto scal = mSelectedNode->GetScale();
			float scalf[3];
			scalf[0] = scal.x;
			scalf[1] = scal.y;
			scalf[2] = scal.z;

			if (ImGui::DragFloat3("Scale", scalf,0.02f)) {
				mSelectedNode->SetScale(float3(scalf[0], scalf[1], scalf[2]));
			}

			auto scripts = mSelectedNode->GetScripts();

			for (int i = 0; i < scripts.size(); i++) {

				ImGui::Text(scripts[i]->name.c_str());

				auto props = mSelectedNode->GetProperties();

				for (int p = 0; p < props.size(); p++)
				{

					auto prop = props[p];

					switch (prop->GetType()) {
					case PropertyType::Int:

					{
						int nv = prop->GetInt();
						if (ImGui::DragInt(prop->GetName().c_str(), &nv)) {

							prop->SetInt(nv);
						}

					}
						break;
					case PropertyType::String:
					{

						char* sbuf = (char*)malloc(512);

						 auto str = prop->GetString();

						 char* act = (char*)str.c_str();




						for (int i = 0; i < 512; i++) {

							auto ch = act[i];

							if (ch == "\0"[0])
							{
								sbuf[i] = ch;
								break;
							}
							sbuf[i] = ch;

						}


						if (ImGui::InputText(prop->GetName().c_str(), sbuf, 512)) {

							prop->SetString(std::string(sbuf));

						}
						else {

							free(sbuf);

						}


					}
						break;

					}
				

				}


			}

			
			ImGui::EndChild();
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Script"))
				{
					DirEntry* entry = (DirEntry*)payload->Data;
					
					mSelectedNode->AddScript(entry->full);

					//node->AddNode(n_node);
					int aa = 5;

					mDragEntry = nullptr;

				}
				ImGui::EndDragDropTarget();
			}

		}

		//exit(1);
		//}

		

	}

	ImGui::End();

}
