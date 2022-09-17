#include "AppZen.h"


AppZen::AppZen() {



}


void AppZen::InitApp() {

	auto io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingWithShift = false;
	io.ConfigDockingAlwaysTabBar = true;

}

void AppZen::UpdateApp() {


}


void AppZen::RenderApp() {

	ImGui::Begin("Scene Viewer");

	ImGui::Text("Testing!");
	ImGui::End();

	

	ImGui::Begin("Zen - Starlight Editor");

	//ImGui::Begin("Scene Viewer");
	ImGui::Text("Below are some settings");

	//ImGui::Checkbox("RayTracing?", &mUseRT);
	//ImGui::Checkbox("Post Processing?", &mUsePP);
	
	ImGui::End();

	
}