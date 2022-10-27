#include "ZenUI.h"


void ZenUI::EditMaterial() {

	if (mEditMatFirst == true)
	{

		ImGui::SetNextWindowPos(ImVec2(128, 128));
		ImGui::SetNextWindowSize(ImVec2(400, 500));
		mEditMatFirst = false;
	}

	auto mat = mEditMaterial;

	bool open = true;

	if (ImGui::Begin("Material",&open)) {

		float dif[3];
		dif[0] = mEditMaterial->GetDiffuse().x;
		dif[1] = mEditMaterial->GetDiffuse().y;
		dif[2] = mEditMaterial->GetDiffuse().z;

		if (ImGui::ColorEdit3("Diffuse", dif))
		{
			mEditMaterial->SetDiffuse(float3(dif[0],dif[1],dif[2]));
		}

		float spec[3];

		spec[0] = mEditMaterial->GetSpecular().x;
		spec[1] = mEditMaterial->GetSpecular().y;
		spec[2] = mEditMaterial->GetSpecular().z;

		if (ImGui::ColorEdit3("Specular", spec))
		{

			mEditMaterial->SetSpecular(float3(spec[0], spec[1], spec[2]));

		}


		auto col_map = mat->GetColorMap();
		auto norm_map = mat->GetNormalMap();
		auto spec_map = mat->GetSpecularMap();


		ImGui::Text("Color"); 
		
		ImGui::Image(col_map->GetView(), ImVec2(64, 64));
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
			{
				DirEntry* entry = (DirEntry*)payload->Data;

				mat->SetColorMap(new Texture2D(entry->full.c_str()));

				mDragEntry = nullptr;

			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear"))
		{
			mat->ClearColorMap();
		}

		ImGui::Text("Normal"); //ImGui::SameLine();
		ImGui::Image(norm_map->GetView(), ImVec2(64, 64));
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
			{
				DirEntry* entry = (DirEntry*)payload->Data;

				mat->SetNormalMap(new Texture2D(entry->full.c_str()));

				mDragEntry = nullptr;

			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();
		if (ImGui::Button("Clear"))
		{
			mat->ClearNormalMap();
		}

		
		ImGui::Text("Specular"); //ImGui::SameLine();
		ImGui::Image(spec_map->GetView(), ImVec2(64, 64));
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
			{
				DirEntry* entry = (DirEntry*)payload->Data;

				mat->SetSpecularMap(new Texture2D(entry->full.c_str()));

				mDragEntry = nullptr;

			}
			ImGui::EndDragDropTarget();
		}


	}

	ImGui::End();

	if (!open) {
		mEditMaterial = nullptr;
		mEditMatFirst = true;
	}

}