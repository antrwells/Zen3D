#include "ZenUI.h"

void ZenUI::MainMenu() {
	//return;

	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("Projects"))
		{

			if (ImGui::MenuItem("Load Project")) {

			}
			if (ImGui::MenuItem("Save Project"))
			{

			}
			if (ImGui::MenuItem("Save Scene"))
			{
				OpenSaveScene();
			}

			if (ImGui::MenuItem("Exit Zen"))
			{
				exit(1);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Renderer"))
		{
			if (ImGui::MenuItem("Post Processing"))
			{
				mPostProcessOpen = true;
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


}