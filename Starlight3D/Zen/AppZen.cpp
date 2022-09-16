#include "AppZen.h"


AppZen::AppZen() {



}


void AppZen::InitApp() {


}

void AppZen::UpdateApp() {


}


void AppZen::RenderApp() {

	ImGui::Begin("Zen - Starlight Editor");

	ImGui::Begin("Scene Viewer");
	ImGui::Text("Below are some settings");

	//ImGui::Checkbox("RayTracing?", &mUseRT);
	//ImGui::Checkbox("Post Processing?", &mUsePP);
	ImGui::End();
	ImGui::End();



}