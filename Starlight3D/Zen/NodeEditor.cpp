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
#include "ZContextVar.h"
#include "ZClassNode.h"

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

			if (mSelectedNode->GetType() == NodeType::Light)
			{

				auto light = (NodeLight*)mSelectedNode;
				
				float lr = light->GetRange();

				if (ImGui::DragFloat("Range", &lr))
				{
					light->SetRange(lr);
				}
				
				float c_diff[3];
				c_diff[0] = light->GetDiffuse().x;
				c_diff[1] = light->GetDiffuse().y;
				c_diff[2] = light->GetDiffuse().z;
				if (ImGui::ColorEdit3("Diffuse", c_diff)) {

					light->SetDiffuse(float3(c_diff[0], c_diff[1], c_diff[2]));

				}

				float c_spec[3];
				c_spec[0] = light->GetSpecular().x;
				c_spec[1] = light->GetSpecular().y;
				c_spec[2] = light->GetSpecular().z;
				if (ImGui::ColorEdit3("Specular", c_spec))
				{
					light->SetSpecular(float3(c_spec[0], c_spec[1], c_spec[2]));
				}

			}

			auto scripts = mSelectedNode->GetScripts();

			for (int i = 0; i < scripts.size(); i++) {

				ImGui::Text(scripts[i]->name.c_str());

				auto vars = scripts[i]->GetVars();

				for (int v = 0; v < vars.size(); v++)
				{

					auto var = vars[v];

					switch (var->GetTypeInt())
					{
					case 0:
					{
						int iv = var->GetIntVal();

						if (ImGui::DragInt(var->GetName().c_str(), &iv))
						{
							var->SetInt(iv);
						}
					}
						break;
					case 1:
					{
						float fv = var->GetFloatVal();
						if (ImGui::DragFloat(var->GetName().c_str(), &fv))
						{
							var->SetFloat(fv);
						}

					}
						break;
					case 2: {

						char* sbuf = (char*)malloc(512);

						auto str = var->GetStringVal();

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


						if (ImGui::InputText(var->GetName().c_str(), sbuf, 512)) {

							var->SetString(std::string(sbuf));


						}
						else {

							free(sbuf);

						}



					}
						  break;
					case 4:
					{
						int bb = 5;
						auto i_cls = var->GetClassVal();
						auto c_name = i_cls->GetBaseName();
						
						if (c_name == "Vec3")
						{

							float v3[3];
							v3[0] = i_cls->FindVar("x")->GetFloatVal();
							v3[1] = i_cls->FindVar("y")->GetFloatVal();
							v3[2] = i_cls->FindVar("z")->GetFloatVal();
							ImGui::DragFloat3(var->GetName().c_str(), v3);
							i_cls->FindVar("x")->SetFloat(v3[0]);
							i_cls->FindVar("y")->SetFloat(v3[1]);
							i_cls->FindVar("z")->SetFloat(v3[2]);


						}


					}
						break;
					}

				}

			}

			/*
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
			*/
			
			ImGui::EndChild();
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Script"))
				{
					DirEntry* entry = (DirEntry*)payload->Data;
					
					VString vs = VString(entry->name.c_str());

					int mp = vs.Find(".");

					VString name = vs.SubString(0, mp);

					int vvv = 5;

					mSelectedNode->AddScript(entry->full,name.GetConst());

					int at = 5;

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
