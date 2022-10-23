#include "AppProjects.h"


AppProjects::AppProjects() {



}

void AppProjects::InitApp() {

	mUI = new ProjectsUI;

}

void AppProjects::UpdateApp() {

}

void AppProjects::RenderApp() {

	mUI->RenderUI();
	//ImGui::Begin("Projects");
	//ImGui::End();

}

void AppProjects::SetPayload() {


}