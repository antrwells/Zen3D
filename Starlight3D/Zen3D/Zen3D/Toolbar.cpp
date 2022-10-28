#include "ZenUI.h"


void ZenUI::MainToolBar() {

	ImVec2 tbpos;
	ImVec2 tbsize;


	ImGuiViewport* viewport = ImGui::GetMainViewport();

	tbpos.x = viewport->Pos.x;
	tbpos.y = viewport->Pos.y + mToolbarSize.y;

	tbsize.x = viewport->Size.x;
	tbsize.y = viewport->Size.y - mToolbarSize.y;

	ImGui::SetNextWindowPos(tbpos);
	ImGui::SetNextWindowSize(tbsize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGuiWindowFlags window_flags = 0
		| ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("Master DockSpace", NULL, window_flags);
	ImGuiID dockMain = ImGui::GetID("MyDockspace");

	// Save off menu bar height for later.
	menuBarHeight = 22;

	ImGui::DockSpace(dockMain);
	ImGui::End();
	ImGui::PopStyleVar(3);

	viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + menuBarHeight));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, mToolbarSize.y));
	ImGui::SetNextWindowViewport(viewport->ID);

	window_flags = 0
		| ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings
		;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::Begin("TOOLBAR", NULL, window_flags);
	ImGui::PopStyleVar();

	if (ImGui::ImageButton(mIconTranslate->GetView(), ImVec2(26, 26)))
	{
		mGizmoMode = GizmoMode::GizmoTranslate;
	}

	ImGui::SameLine();
	if (ImGui::ImageButton(mIconRotate->GetView(), ImVec2(26, 26))) {

		mGizmoMode = GizmoMode::GizmoRotate;

	}

	ImGui::SameLine();
	if (ImGui::ImageButton(mIconScale->GetView(), ImVec2(26, 26))) {

		mGizmoMode = GizmoMode::GizmoScale;

	}

	//	ImGui::Separator();



	ImGui::SameLine();
	//ImGui::SetNextWindowSize(ImVec2(64, 28));
	ImGui::PushItemWidth(128);
	//if (ImGui::BeginCombo("Transform Space","Transform Space")) {
		//ImGui::Combo("Local Space",&mSpaceItem,"Local Space 0 Global Space",32);
	const char* items[] = { "Local Space", "World Space" };
	ImGui::Combo("Transform Space", &mSpaceItem, items, 2);
	//ImGui::Combo("Global Space",&mSpaceItem,"");
	//	ImGui::EndCombo();
	//}
	switch (mSpaceItem) {
	case 0:

		mGizmoSpace = GizmoSpace::Local;

		break;
	case 1:

		mGizmoSpace = GizmoSpace::Global;

		break;
	}

	ImGui::PopItemWidth();

	ImGui::SameLine();
	ImGui::Separator();
	ImGui::SameLine();
	//ImGui::Button("Check");
	if (ImGui::ImageButton(mIconPlay->GetView(), ImVec2(26, 26)))
	{

		BeginPlay();
	}

	ImGui::SameLine();

	if (ImGui::ImageButton(mIconStop->GetView(), ImVec2(26, 26)))
	{
		EndPlay();
	}

	ImGui::SameLine();


	if (mPlaying == false) {
		ImGui::PushItemWidth(128);



		std::vector<char*> cam_names;
		std::vector<NodeCamera*> cams = mGraph->GetCams();

		for (int i = 0; i < cams.size(); i++) {
			cam_names.push_back((char*)cams[i]->GetName());
		}

		const char* citems[] = { "Edit Caemera", "Game Camera" };
		ImGui::Combo("Viewport Camera", &mCurrentCamID, cam_names.data(), cams.size());

		mCurrentCamera = cams[mCurrentCamID];
		mGraph->SetCamera(mCurrentCamera);


		ImGui::PopItemWidth();
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Editor Grid", &mGridOn))
	{

	}

	ImGui::End();

}
