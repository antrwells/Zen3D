#include "ProjectsUI.h"
#include <stdlib.h>
#include <malloc.h>
#include <process.h>
#include "VFile.h"


ProjectsUI::ProjectsUI() {

	fileDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_SelectDirectory);
	fileDialog.SetTitle("Select path for Project.");
	if (VFile::Exists("projects.list"))
	{
		LoadList();
	}
	else {
		SaveList();
	}

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
			auto open_proj = new VFile("C:\\ZenEditor\\Project.load", FileMode::Write);
			open_proj->WriteString(mCurrentProject->GetPath().c_str());
			open_proj->Close();
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
			SaveList();
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

		if (ImGui::InputText("Project Name", (char*)name, 512)) {
			mCurrentProject->SetName(std::string(name));
			SaveList();
		}

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

		if (ImGui::InputText("Project Path", (char*)path, 512))
		{

		
			mCurrentProject->SetPath(std::string(path));
			SaveList();
		}
	

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
			SaveList();

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
			SaveList();
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
		SaveList();

	}

}

void ProjectsUI::NewProject() {

	ZenProject* new_proj = new ZenProject;
	mProjects.push_back(new_proj);
	mCurrentProject = new_proj;
	SaveList();

}

void ProjectsUI::SaveList()
{

	VFile* projList = new VFile("projects.list", FileMode::Write);

	projList->WriteInt(mProjects.size());
	for (int i = 0; i < mProjects.size(); i++) {

		auto proj = mProjects[i];
		projList->WriteString(proj->GetName().c_str());
		projList->WriteString(proj->GetPath().c_str());
		projList->WriteString(proj->GetAuthor().c_str());
		projList->WriteString(proj->GetInfo().c_str());


	}

	projList->Close();

}

void ProjectsUI::LoadList() {

	VFile* list = new VFile("projects.list", FileMode::Read);

	int pc = list->ReadInt();
	for (int i = 0; i < pc; i++) {

		ZenProject* proj = new ZenProject;
		proj->SetName(list->ReadString());
		proj->SetPath(list->ReadString());
		proj->SetAuthor(list->ReadString());
		proj->SetInfo(list->ReadString());
		mProjects.push_back(proj);
	}

	list->Close();

}