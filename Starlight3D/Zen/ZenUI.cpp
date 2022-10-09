#include "ZenUI.h"
#include "UITheme_Neon.h"
#include "Importer.h"
#include "RayPicker.h"
#include "VString.h"
#include "DirCollection.h"
#include "VString.h"
#include "VFile.h"

ZenUI::ZenUI(SceneGraph* graph) {

	mScriptNameBuf = (char*)malloc(512);
	mScriptNameBuf[0] = "\0"[0];

	auto importer = new Importer;

	mGraph = graph;

	mMainCamera = graph->GetCamera();


	//Toolbar
	mToolbarPos = ImVec2(0, 22);
	mToolbarSize = ImVec2(Application::GetApp()->GetWidth(), 54);

	//Icons

	mIconTranslate = new Texture2D("edit/icon/MoveIcon2.png");
	mIconRotate = new Texture2D("edit/icon/rotateIcon.png");
	mIconScale = new Texture2D("edit/icon/scaleicon.png");
	mIconPlay = new Texture2D("edit/icon/playicon.png");
	mIconStop = new Texture2D("edit/icon/stopicon.png");

	//Scene Graph
	mSceneGraphPos = ImVec2(0, mToolbarSize.y+12);
	mSceneGraphSize = ImVec2(240, Application::GetApp()->GetHeight()-22-200-mToolbarSize.y);

	//Scene Viewport Resources
	mSceneViewPos = ImVec2(240, mToolbarSize.y+12);
	mSceneViewSize = ImVec2(750, Application::GetApp()->GetHeight() - 22-200-mToolbarSize.y);
	mRenderTarget = new RenderTarget2D(mSceneViewSize.x, mSceneViewSize.y);
	cam_rotation = ImVec2(0, 0);
	mTranslateGizmo = importer->ImportAI("edit/gizmo/translate1.fbx");
	mRotateGizmo = importer->ImportAI("edit/gizmo/rotate1.fbx");
	mScaleGizmo = importer->ImportAI("edit/gizmo/scale1.fbx");
	mTranslateGizmo->GetMesh(0)->SetName("Z");
	mTranslateGizmo->GetMesh(1)->SetName("X");
	mTranslateGizmo->GetMesh(2)->SetName("Y");
	mRotateGizmo->GetMesh(2)->SetName("X");
	mRotateGizmo->GetMesh(0)->SetName("Y");
	mRotateGizmo->GetMesh(1)->SetName("Z");
	mScaleGizmo->GetMesh(0)->SetName("X");
	mScaleGizmo->GetMesh(1)->SetName("Y");
	mScaleGizmo->GetMesh(2)->SetName("Z");
	auto red_tex = new Texture2D("edit/gizmo/red.png");
	auto blue_tex = new Texture2D("edit/gizmo/blue.png");
	auto green_tex = new Texture2D("edit/gizmo/green.png");

	mTranslateGizmo->GetMesh(0)->GetMaterial()->SetColorMap(blue_tex);
	mTranslateGizmo->GetMesh(1)->GetMaterial()->SetColorMap(red_tex);
	mTranslateGizmo->GetMesh(2)->GetMaterial()->SetColorMap(green_tex);

	mRotateGizmo->GetMesh(0)->GetMaterial()->SetColorMap(green_tex);
	mRotateGizmo->GetMesh(1)->GetMaterial()->SetColorMap(blue_tex);
	mRotateGizmo->GetMesh(2)->GetMaterial()->SetColorMap(red_tex);

	mScaleGizmo->GetMesh(0)->GetMaterial()->SetColorMap(red_tex);
	mScaleGizmo->GetMesh(1)->GetMaterial()->SetColorMap(green_tex);
	mScaleGizmo->GetMesh(2)->GetMaterial()->SetColorMap(blue_tex);



	//Scene Viewport Globals
	gLock_x = gLock_y = gLock_z = false;
	mGizmoMode = GizmoMode::GizmoTranslate;
	mGizmoSpace = GizmoSpace::Global;


	//mGraph->AddNode(mTranslateGizmo);
	mTranslateGizmo->SetPosition(float3(0, 1, 0));
	mRotateGizmo->SetPosition(float3(0, 1, 0));
	mScaleGizmo->SetPosition(float3(0, 1, 0));


	//Content Browser
	mContentBrowserPos = ImVec2(0, 12+Application::GetApp()->GetHeight() - 200-22);
	mContentBrowserSize = ImVec2(Application::GetApp()->GetWidth(), 222-12);// mToolbarSize.y);

	//Icons

	mIconFolder = new Texture2D("edit/browser/foldericon.png");
	mIconFile = new Texture2D("edit/browser/fileicon.png");
	mIconScript = new Texture2D("edit/browser/scripticon.png");

	//Node Editor
	mNodeEditPos = ImVec2(990,mToolbarSize.y+12);
	mNodeEditSize = ImVec2(Application::GetApp()->GetWidth() -990, Application::GetApp()->GetHeight() - 22 - 200-mToolbarSize.y);


	//Other Resources

	mRayPick = new RayPicker(mGraph);

	mToolbarSize.y = mToolbarSize.y - 8;


	ScanContent("c:/ZenContent/");

}

void ZenUI::MainWindow() {

	MainBGWindow();

	MainMenu();

	SceneGraphWindow();

	MainViewPort();

	MainContentBrowser();

	MainNodeEditor();



}

void ZenUI::MainMenu() {
	//return;

	if (ImGui::BeginMainMenuBar()) {

		if(ImGui::BeginMenu("Projects"))
		{

			if (ImGui::MenuItem("Load Project")) {

			}
			if (ImGui::MenuItem("Save Projects"))
			{

			}

			if (ImGui::MenuItem("Exit Zen"))
			{
				exit(1);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
		

}




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

	if (ImGui::ImageButton(mIconTranslate->GetView() , ImVec2(26,26)))
	{
		mGizmoMode = GizmoMode::GizmoTranslate;
	}

	ImGui::SameLine();
	if (ImGui::ImageButton(mIconRotate->GetView(), ImVec2(26, 26))) {

		mGizmoMode = GizmoMode::GizmoRotate;

	}

	ImGui::SameLine();
	if(ImGui::ImageButton(mIconScale->GetView(), ImVec2(26, 26))) {

		mGizmoMode = GizmoMode::GizmoScale;

	}

//	ImGui::Separator();



	ImGui::SameLine();
	//ImGui::SetNextWindowSize(ImVec2(64, 28));
	ImGui::PushItemWidth(128);
	//if (ImGui::BeginCombo("Transform Space","Transform Space")) {
		//ImGui::Combo("Local Space",&mSpaceItem,"Local Space 0 Global Space",32);
	const char* items[] = { "Local Space", "World Space" };
		ImGui::Combo("Transform Space",&mSpaceItem,items,2);
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

	ImGui::End();

}



/// Main BG Window

void ZenUI::MainBGWindow()
{

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	return;
	int flags = ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoCollapse;
	//flags |= ImGuiWindowFlags_NoDecoration;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	//flags != ImGuiWindowFlags_NoTitleBar;

	ImGui::SetNextWindowPos(ImVec2(0, 23));
	ImGui::SetNextWindowSize(ImVec2(Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight()));
	bool open = true;
	ImGui::Begin("Zen", &open, flags);

	ImGui::End();
}


void ZenUI::ScanContent(std::string path) {

	mContentPath = new VString(path.c_str());
	mDir = new DirCollection(path);
	int bb = 5;

}

///Othr functionality

Node3D* ZenUI::ImportNode(const char* path) {

	Importer* importer = new Importer;
	auto node = importer->ImportAI(path);
	//mGraph->AddNode(node);
	return node;
}

// Get script name

void ZenUI::GetScriptName() {

	ImGui::SetNextWindowSize(ImVec2(512, 80));
	if (ImGui::Begin("Script Name"))
	{

		ImGui::InputText("Script Name",(char*)mScriptNameBuf, 512);
		if (ImGui::Button("Create"))
		{
			mGetScriptName = false;
			VString s_path = VString(mContentPath->GetConst());
			s_path.Add(VString("/"));
			s_path.Add(VString(mScriptNameBuf));


			VFile* file = new VFile(s_path.GetConst(), FileMode::Write);
		//	file->WriteLine("This is a test.\n");
		//	file->WriteLine("and so is this.\n");
			file->Close();

		
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			mGetScriptName = false;
		}
	}

	ImGui::End();

}


//////////
void ZenUI::UpdateUI() {

	if (mPlaying) {
		mGraph->Update();
	}

}

void ZenUI::RenderUI() {
	 
	MainToolBar();
	MainWindow();
	if (mGetScriptName) {

		GetScriptName();

	}

}

