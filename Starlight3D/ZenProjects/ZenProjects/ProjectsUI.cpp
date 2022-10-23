#include "ProjectsUI.h"
#include <stdlib.h>
#include <malloc.h>
#include <process.h>


ProjectsUI::ProjectsUI() {

	fileDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_SelectDirectory);
	fileDialog.SetTitle("Select path for Project.");

}

void ProjectsUI::RenderUI() {

	RenderProjectList();

}

void ProjectsUI::RenderProjectList() {


	ImGui::SetNextWindowPos(ImVec2(272, 0));
	ImGui::SetNextWindowSize(ImVec2(530, 450));
	ImGui::Begin("Commands");
	ImGui::BeginChild(20);

	if (ImGui::Button("New Project"))
	{
		fileDialog.Open();
		//NewProject();

	}

	if (mCurrentProject != nullptr) {


		ImGui::SameLine();
		if (ImGui::Button("Load Project"))
		{
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));
			//const char* zenPath = "C:\ZenEditor\Zen3D.exe";
			
			auto path = "C:\\ZenEditor\\Zen3D.exe";
			system(path);
			

		}
		ImGui::SameLine();
		if (ImGui::Button("Remove Project"))
		{
			for (int i = 0; i < mProjects.size(); i++) {
				if (mProjects[i] == mCurrentProject) {
					mProjects.erase(mProjects.begin() + i);
					if (mProjects.size() > 0) {
						mCurrentProject = mProjects[0];
					}
					else {
						mCurrentProject = nullptr;
					}

				}
			}
		}

	}

	if (mCurrentProject != nullptr) {

		char* name = (char*)malloc(1024);

		auto name2 = mCurrentProject->GetName();

		for (int i = 0; i < 512; i++) {

			int ch = name2[i];
			if (ch == "\0"[0])
			{
				name[i] = "\0"[0];
				break;
			}
			name[i] = ch;

		}

		ImGui::InputText("Project Name", (char*)name, 512);
		mCurrentProject->SetName(std::string(name));

		char* path = (char*)malloc(1024);

		auto path2 = mCurrentProject->GetPath();

		for (int i = 0; i < 512; i++) {

			int ch = path2[i];
			if (ch == "\0"[0])
			{
				path[i] = "\0"[0];
				break;
			}
			path[i] = ch;

		}

		ImGui::InputText("Project Path", (char*)path, 512);
		mCurrentProject->SetPath(std::string(path));


		char* info = (char*)malloc(4096);

		auto info2 = mCurrentProject->GetInfo();
		for (int i = 0; i < 512; i++) {

			int ch = info2[i];
			if (ch == "\0"[0])
			{
				info[i] = "\0"[0];
				break;
			}
			info[i] = ch;

		}



		if (ImGui::InputTextMultiline("Project Info", (char*)info, 4096)) {

			mCurrentProject->SetInfo(info);

		}

		char* author = (char*)malloc(512);
		auto author2 = mCurrentProject->GetAuthor();
		for (int i = 0; i < 512; i++) {

			int ch = author2[i];
			if (ch == "\0"[0])
			{
				author[i] = "\0"[0];
				break;
			}
			author[i] = ch;

		}

		if (ImGui::InputText("Project Author", (char*)author,512))
		{

			mCurrentProject->SetAuthor(author);

		}

	}

	ImGui::EndChild();
	ImGui::End();

	
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(270, 450));

	ImGui::Begin("Projects");

	ImGui::BeginChild(25);

	for (int i = 0; i < mProjects.size(); i++) {
		
		auto proj = mProjects[i];

		auto proj_name = proj->GetName();
		proj_name = "Project:" + proj_name;

		if (ImGui::TreeNodeEx(proj_name.c_str()))
		{
			if (ImGui::IsItemClicked()) {

				mCurrentProject = proj;

			}

			ImGui::TreePop();
		}

	}
	ImGui::EndChild();

	ImGui::End();

	fileDialog.Display();

	if (fileDialog.HasSelected()) {

		auto path = fileDialog.GetSelected();

		NewProject();
		mCurrentProject->SetPath(path.string());

		int b = 5;
		fileDialog.ClearSelected();
		mCurrentProject->SetName("New Project");

	}

}

void ProjectsUI::NewProject() {

	ZenProject* new_proj = new ZenProject;
	mProjects.push_back(new_proj);
	mCurrentProject = new_proj;

}