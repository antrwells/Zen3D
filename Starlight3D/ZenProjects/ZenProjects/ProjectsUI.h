#pragma once
#include "Application.h"
#include "ZenProject.h"
#include "imfilebrowser.h"
class ProjectsUI
{
public:

	ProjectsUI();


	
	void RenderUI();
	void RenderProjectList();

	void NewProject();

	void SaveList();
	void LoadList();

private:

	std::vector<ZenProject*> mProjects;
	ZenProject* mCurrentProject = nullptr;
	ImGui::FileBrowser fileDialog;
};

