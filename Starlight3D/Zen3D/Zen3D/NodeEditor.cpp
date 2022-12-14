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
struct NodeRef {

	Node3D* node = nullptr;

};

bool node_vis = true;
bool light_vis = true;
bool entity_vis = true;
bool actor_vis = true;
bool cam_vis = true;
bool* s_vis = new bool[256];
bool init_s = true;
void ZenUI::MainNodeEditor() {

	if (init_s) {
		for (int i = 0;i < 256;i++) {
			s_vis[i] = true;
		}
		init_s = false;
	}
	if (!mNEF)
	{
		ImGui::SetNextWindowPos(mNodeEditPos);
		ImGui::SetNextWindowSize(mNodeEditSize);
		mNEF = true;
	}

	int flags = ImGuiWindowFlags_MenuBar;

	if (ImGui::Begin("Node Editor"))
	{


	

		if (mEditNode != nullptr) {
			//ImGui::Text(mEditNode->GetName());

		

			ImGui::BeginChild(55);

				if (ImGui::CollapsingHeader("Node", &node_vis))
				{

				
				bool enable = mEditNode->GetEnabled();

				if (ImGui::Checkbox("Enabled", &enable)) {

					mEditNode->SetEnabled(enable);

				}

				char* buf = (char*)malloc(512);
				auto name = mEditNode->GetName();

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

					mEditNode->SetName(buf);

				}
				else {
					free(buf);
				}


				auto pos = mEditNode->GetPosition();
				float posf[3];
				posf[0] = pos.x;
				posf[1] = pos.y;
				posf[2] = pos.z;

				if (ImGui::DragFloat3("Position", posf, 0.1f)) {
					mEditNode->SetPosition(float3(posf[0], posf[1], posf[2]));
				}


				auto scal = mEditNode->GetScale();
				float scalf[3];
				scalf[0] = scal.x;
				scalf[1] = scal.y;
				scalf[2] = scal.z;

				if (ImGui::DragFloat3("Scale", scalf, 0.02f)) {
					mEditNode->SetScale(float3(scalf[0], scalf[1], scalf[2]));
				}
			}
			int pmode = 0;
		


			if (mEditNode->GetType() == NodeType::Entity) {
		


			}



			if (mEditNode->GetType() == NodeType::Camera) {

				auto cam = (NodeCamera*)mEditNode;

				if (ImGui::CollapsingHeader("Camera", &cam_vis)) {




					float ce[2];
					ce[0] = cam->GetMinZ();
					ce[1] = cam->GetMaxZ();
					if (ImGui::DragFloat2("Camera Extents", ce, 0.01f, 0.0001, 1024)) {
						cam->SetMinZ(ce[0]);
						cam->SetMaxZ(ce[1]);
					}

					float fov = cam->GetFOV();
					if (ImGui::DragFloat("Field Of View", &fov, 0.1f, 1, 180))
					{
						cam->SetFOV(fov);
					}

					ImGui::Text("Lens Image");
					ImGui::Image(cam->GetLensImage()->GetView(), ImVec2(64, 64));
					if (ImGui::BeginDragDropTarget()) {
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
						{
							DirEntry* entry = (DirEntry*)payload->Data;

							cam->SetLensImage(new Texture2D(entry->full.c_str()));
							//mat->SetSpecularMap(new Texture2D(entry->full.c_str()));

							mDragEntry = nullptr;

						}
						ImGui::EndDragDropTarget();
					}
				}
			}
			if (mEditNode->GetType() == NodeType::Light)
			{


				if (ImGui::CollapsingHeader("Light", &light_vis))
				{
					auto light = (NodeLight*)mEditNode;

					int lightType = (int)light->GetLightType();

					ImGui::PushItemWidth(128);
					//if (ImGui::BeginCombo("Transform Space","Transform Space")) {
						//ImGui::Combo("Local Space",&mSpaceItem,"Local Space 0 Global Space",32);
					const char* items[] = { "Point", "Spot","Directional" };
					ImGui::Combo("Light Type", &lightType, items, 3);
					//ImGui::Combo("Global Space",&mSpaceItem,"");
				//	ImGui::EndCombo();
				//}
					switch (lightType) {
					case 0:

						//mGizmoSpace = GizmoSpace::Local;
						light->SetLightType(LightType::PointLight);

						break;
					case 1:

					{
						light->SetLightType(LightType::SpotLight);

						auto cone = light->GetCone();

						float ic, oc;

						ic = cone.x;
						oc = cone.y;

						if (ImGui::DragFloat("Cone", &ic, 0.15f, 0, 300))
						{
							cone.x = ic;
						}

						//if (ImGui::DragFloat("Outter Cone", &oc, 0.15f, 300)) {
					//		cone.y = oc;
					//	}
						light->SetCone(cone);

						//mGizmoSpace = GizmoSpace::Global;
					}
					break;
					case 2:
						light->SetLightType(LightType::DirectionalLight);
						break;
					}

					ImGui::PopItemWidth();


					ImGui::SameLine();
					ImGui::PushItemWidth(72);
					float lr = light->GetRange();

					if (ImGui::DragFloat("Range", &lr))
					{
						light->SetRange(lr);
					}
					ImGui::PopItemWidth();


					bool castShadows = light->GetCastShadows();
					if (ImGui::Checkbox("Cast Shadows", &castShadows))
					{
						light->SetCastShadows(castShadows);
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
			}

			auto scripts = mEditNode->GetScripts();

			for (int i = 0; i < scripts.size(); i++) {



				if (ImGui::CollapsingHeader(scripts[i]->name.c_str(), &s_vis[i]))
				{


					//ImGui::Text(scripts[i]->name.c_str());

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
							if (ImGui::BeginDragDropTarget()) {
								if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
								{
									DirEntry* entry = (DirEntry*)payload->Data;
									var->SetString(entry->full.c_str());
									//cam->SetLensImage(new Texture2D(entry->full.c_str()));
									//mat->SetSpecularMap(new Texture2D(entry->full.c_str()));

									mDragEntry = nullptr;

								}
								else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Sound")) {
									DirEntry* entry = (DirEntry*)payload->Data;
									var->SetString(entry->full.c_str());
									//cam->SetLensImage(new Texture2D(entry->full.c_str()));
									//mat->SetSpecularMap(new Texture2D(entry->full.c_str()));

									mDragEntry = nullptr;


								}
								else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Scene")) {
									DirEntry* entry = (DirEntry*)payload->Data;
									var->SetString(entry->full.c_str());
									//cam->SetLensImage(new Texture2D(entry->full.c_str()));
									//mat->SetSpecularMap(new Texture2D(entry->full.c_str()));

									mDragEntry = nullptr;


								}
								else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Movie")) {
									DirEntry* entry = (DirEntry*)payload->Data;
									var->SetString(entry->full.c_str());
									//cam->SetLensImage(new Texture2D(entry->full.c_str()));
									//mat->SetSpecularMap(new Texture2D(entry->full.c_str()));

									mDragEntry = nullptr;


								} if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Cine")) {
									DirEntry* entry = (DirEntry*)payload->Data;
									var->SetString(entry->full.c_str());
									//cam->SetLensImage(new Texture2D(entry->full.c_str()));
									//mat->SetSpecularMap(new Texture2D(entry->full.c_str()));

									mDragEntry = nullptr;


								}
								ImGui::EndDragDropTarget();
							}


						}
							  break;
						case 4:
						{
							int bb = 5;
							auto i_cls = var->GetClassVal();
							//if (i_cls == nullptr) continue
							if (i_cls != nullptr) {
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
									break;

								}
								else {
									std::string vn = std::string("Script:");
									auto bt = var->GetBaseID();
									vn = vn + bt;
									ImGui::InputText(var->GetName().c_str(), (char*)vn.c_str(), 256);
								}
							}
							else {


								//****
								std::string vn = std::string("Empty:");
								auto bt = var->GetBaseID();
								vn = vn + bt;
								ImGui::InputText(var->GetName().c_str(), (char*)vn.c_str(), 256);
								if (ImGui::BeginDragDropTarget()) {

									if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Node"))
									{
										auto nr = (NodeRef*)payload->Data;
										auto node2 = nr->node;

										auto scripts = node2->GetScripts();
										if (scripts.size() == 0) {
											return;
										}
										auto s1 = scripts[0];
										auto cn = s1->mClsName;
										var->SetClass(s1->mMainClass);

										//node2->GetRootNode()->RemoveNode(node2);
										//node->AddNode(node2);


									}
									ImGui::EndDragDropTarget();
								}

							}


						}
						break;
						}

					}
				}
			}

			/*
			auto scripts = mEditNode->GetScripts();

			for (int i = 0; i < scripts.size(); i++) {

				ImGui::Text(scripts[i]->name.c_str());

				auto props = mEditNode->GetProperties();

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


			//Material
			if (mEditNode->GetType() == NodeType::Actor) {

				if (ImGui::CollapsingHeader("Actor", &actor_vis)) {


					auto ent = (NodeActor*)mEditNode;
					//	ImGui::Text("Physics Type");

					int pType = (int)ent->GetPhysicsType();

					ImGui::PushItemWidth(128);
					//if (ImGui::BeginCombo("Transform Space","Transform Space")) {
						//ImGui::Combo("Local Space",&mSpaceItem,"Local Space 0 Global Space",32);


					const char* items[] = { "None", "Box", "Sphere","Capsule","Convex","Mesh" };

					if (ImGui::Combo("Physics Type", &pType, items, 6)) {

						ent->SetPhysicsType((PhysicsType)pType);

					}


					ImGui::PopItemWidth();

					auto entity = (NodeEntity*)mEditNode;

					auto meshes = entity->GetMeshes();

					for (int m = 0; m < meshes.size(); m++)
					{

						VString name("Mesh:");
						name.Add(VString(m));

						ImGui::Text(name.GetConst());

						auto mesh = meshes[m];
						auto mat = mesh->GetMaterial();


						ImGui::SameLine();
						if (ImGui::Button("Edit Material"))
						{

							mEditMaterial = mat;

						}

						if (ImGui::Button("Edit Animations"))
						{
							mEditAnimation = (NodeActor*)entity;
						}

					}
				}
			}

			if (mEditNode->GetType() == NodeType::Entity)
			{
				auto entity = (NodeEntity*)mEditNode;
				auto ent = (NodeEntity*)mEditNode;
				//	ImGui::Text("Physics Type");

				int pType = (int)ent->GetPhysicsType();
				ImGui::PushItemWidth(128);
				//if (ImGui::BeginCombo("Transform Space","Transform Space")) {
					//ImGui::Combo("Local Space",&mSpaceItem,"Local Space 0 Global Space",32);


				const char* items[] = { "None", "Box", "Sphere","Capsule","Convex","Mesh" };

				if (ImGui::Combo("Physics Type", &pType, items, 6)) {

					ent->SetPhysicsType((PhysicsType)pType);

				}


				ImGui::PopItemWidth();

				auto meshes = entity->GetMeshes();

				for (int m = 0; m < meshes.size(); m++)
				{

					VString name("Mesh:");
					name.Add(VString(m));

					ImGui::Text(name.GetConst());

					auto mesh = meshes[m];
					auto mat = mesh->GetMaterial();


					ImGui::PushID(128 + m);
					ImGui::SameLine();
					if (ImGui::Button("Edit Material"))
					{

						mEditMaterial = mat;

					}
					ImGui::PopID();


				}
			}

			ImGui::EndChild();
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Script"))
				{
					DirEntry* entry = (DirEntry*)payload->Data;

					VString vs = VString(entry->name.c_str());

					int mp = vs.Find(".");

					VString name = vs.SubString(0, mp);

					int vvv = 5;

					mEditNode->AddScript(entry->full, name.GetConst());

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


