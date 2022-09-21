#include "ZenUI.h"
#include "UITheme_Neon.h"
#include "Importer.h"
#include "RayPicker.h"
#include "VString.h"

ZenUI::ZenUI(SceneGraph* graph) {


	auto importer = new Importer;

	mGraph = graph;

	mMainCamera = graph->GetCamera();

	//Scene Graph
	mSceneGraphPos = ImVec2(0, 22);
	mSceneGraphSize = ImVec2(240, Application::GetApp()->GetHeight()-22-200);

	//Scene Viewport Resources
	mSceneViewPos = ImVec2(240, 22);
	mSceneViewSize = ImVec2(750, Application::GetApp()->GetHeight() - 22-200);
	mRenderTarget = new RenderTarget2D(mSceneViewSize.x, mSceneViewSize.y);
	cam_rotation = ImVec2(0, 0);
	mTranslateGizmo = importer->ImportAI("edit/gizmo/translate1.fbx");
	mTranslateGizmo->GetMesh(0)->SetName("Z");
	mTranslateGizmo->GetMesh(1)->SetName("X");
	mTranslateGizmo->GetMesh(2)->SetName("Y");
	auto red_tex = new Texture2D("edit/gizmo/red.png");
	auto blue_tex = new Texture2D("edit/gizmo/blue.png");
	auto green_tex = new Texture2D("edit/gizmo/green.png");

	mTranslateGizmo->GetMesh(0)->GetMaterial()->SetColorMap(blue_tex);
	mTranslateGizmo->GetMesh(1)->GetMaterial()->SetColorMap(red_tex);
	mTranslateGizmo->GetMesh(2)->GetMaterial()->SetColorMap(green_tex);

	//Scene Viewport Globals
	gLock_x = gLock_y = gLock_z = false;
	mGizmoMode = GizmoMode::GizmoTranslate;

	//mGraph->AddNode(mTranslateGizmo);
	mTranslateGizmo->SetPosition(float3(0, 1, 0));


	//Content Browser
	mContentBrowserPos = ImVec2(0, Application::GetApp()->GetHeight() - 200);
	mContentBrowserSize = ImVec2(Application::GetApp()->GetWidth(), 222);

	//Node Editor
	mNodeEditPos = ImVec2(990, 22);
	mNodeEditSize = ImVec2(Application::GetApp()->GetWidth() -990, Application::GetApp()->GetHeight() - 22 - 200);


	//Other Resources

	mRayPick = new RayPicker(mGraph);


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
bool z = false;

int node_id = 0;

void ZenUI::SceneTree(Node3D* node)
{

	ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;



	if (node == mSelectedNode) {

		//flags = ImGuiTreeNodeFlags_Selected;
		printf("S>>>");
		printf(node->GetName());
		printf("\n");

	}

	if (node->ChildrenCount() == 0) {

		flag |= ImGuiTreeNodeFlags_Leaf;

	}

	if (node == mSelectedNode)
	{
		flag |= ImGuiTreeNodeFlags_Selected;
	}

	if (ImGui::TreeNodeEx(node->GetName(), flag)) {

		
		if (ImGui::IsItemClicked()) {
			mSelectedNode = node;
		}

	
		if (node->ChildrenCount() > 0) {
			
			for (int i = 0;i < node->ChildrenCount();i++) {

				node_id++;
				SceneTree(node->GetChild(i));

			}
		}
	
	
		//printf("Selected Node!");

		ImGui::TreePop();
	}
	else {

		

	}


}

void ZenUI::SceneGraphWindow() {

	
	if (!mSGF) {


		ImGui::SetNextWindowPos(mSceneGraphPos);
		ImGui::SetNextWindowSize(mSceneGraphSize);
		mSGF = true;
	}

	ImGui::Begin("Scene Graph", &mSceneGraphOpen, ImGuiWindowFlags_MenuBar);
	ImGui::BeginChild("SceneTree");
	
	node_id = 0;
	SceneTree(mGraph->GetRoot());
	if (mSelectedNode != nullptr) {
		
		printf("Selected Node:");
		printf(mSelectedNode->GetName());
printf("\n");
	}


	ImGui::EndChild();
	ImGui::End();

}



void ZenUI::MainToolBar() {


}



void ZenUI::MainViewPort() {

	if (!mSVF) {


		ImGui::SetNextWindowPos(mSceneViewPos);
		ImGui::SetNextWindowSize(mSceneViewSize);
		mSVF = true;

	}

	auto io = ImGui::GetIO();
	io.WantCaptureMouse = true;
	//ImVec2 mp = io.MousePos;

	ImVec2 mp = ImGui::GetMousePos();


	//printf("MX:%d MY:%d\n", (int)mp.x, (int)mp.y);
	int flags = 0;




	if (ImGui::Begin("Scene Viewer", &mSceneViewOpen, flags)) {

		ImVec2 win_pos = ImGui::GetWindowPos();
		ImVec2 win_size = ImGui::GetWindowSize();

		if ((int)(win_size.x) != mRenderTarget->GetWidth() || (int)(win_size.y) != mRenderTarget->GetHeight()) {

			mRenderTarget = new RenderTarget2D((int)win_size.x, (int)win_size.y);
			printf("============================================================>\n");
			auto cam = mGraph->GetCamera();
			cam->SetViewport(0, 0, win_size.x, win_size.y);


		}

		ImVec2 real_pos;
		real_pos.x = mp.x - win_pos.x;
		real_pos.y = mp.y - win_pos.y;
		//real_pos.x = real_pos.x - 4;
		real_pos.y = real_pos.y - 20;

		printf("Ox:%d Oy:%d\n", (int)real_pos.x, (int)real_pos.y);

		//ImVec2 delta;


		mouse_delta.x = real_pos.x - prev_mouse.x;
		mouse_delta.y = real_pos.y - prev_mouse.y;

		prev_mouse = real_pos;

		if (real_pos.x > 0 && real_pos.y > 0 && real_pos.x < win_size.x && real_pos.y < win_size.y)
		{

			if (Application::GetApp()->GetInput()->IsMouseDown(1))
			{
				cam_interact = true;
			}
			else {
				cam_interact = false;
			}

			if (Application::GetApp()->GetInput()->IsMouseDown(0)) {

				if (gLock) {

					if (mSelectedNode != nullptr) {
						
						float3 new_pos = mSelectedNode->GetPosition();

						if (gLock_x)
						{

							new_pos.x += ((float)Application::GetApp()->GetInput()->GetMouseDX()) * mTranslateRatio;
							mSelectedNode->SetPosition(new_pos);
						}
						if (gLock_y) {

							new_pos.y += ((float)-Application::GetApp()->GetInput()->GetMouseDY()) * mTranslateRatio;
							mSelectedNode->SetPosition(new_pos);

						}
						if (gLock_z) {

							new_pos.z += ((float)-Application::GetApp()->GetInput()->GetMouseDY()) * mTranslateRatio;
							mSelectedNode->SetPosition(new_pos);

						}

					}

				}
				else {
					bool check_rest = true;
					if (mSelectedNode != nullptr) {
						auto giz_result = mRayPick->MousePickNode((int)real_pos.x, (int)real_pos.y, (int)win_size.x, (int)win_size.y - 28, mCurrentGizmo, mMainCamera);

						if (giz_result.hit) {
							VString giz_name = VString(giz_result.hit_mesh->GetName());

							if (giz_name.Contains(VString("X")))
							{
								gLock_x = true;
								gLock_y = false;
								gLock_z = false;
								gLock = true;
							}
							if (giz_name.Contains(VString("Y")))
							{
								gLock_x = false;
								gLock_y = true;
								gLock_z = false;
								gLock = true;
							}
							if (giz_name.Contains("Z")) {
								gLock_x = false;
								gLock_y = false;
								gLock_z = true;
								gLock = true;
							}
						}
						if (giz_result.hit) {

							int bb = 5;

							switch (mGizmoMode) {

							case GizmoMode::GizmoTranslate:



								break;

							}

							check_rest = false;
						}

					}

					if (check_rest) {
						auto result = mRayPick->MousePick((int)real_pos.x, (int)real_pos.y, (int)win_size.x, (int)win_size.y - 28, mMainCamera);

						if (result.hit) {
							int bb = 5;
							if (result.hit_entity != mTranslateGizmo) {

								mSelectedNode = result.hit_entity;
								//mTranslateGizmo->SetPosition(result.hit_node->GetPosition());

							}
							//mTranslateGizmo->SetPosition(result.hit_point);
							//printf("=========================================<<<<<<<<<<<<<<<<<<<<<<");
						}
					}
				}
				//exit(1);
			}
			else {
				gLock = false;
				gLock_x = false;
				gLock_y = false;
				gLock_z = false;
			}
			

		}
		else {
			cam_interact = false;
		}

		mGraph->RenderShadowMaps();
		mRenderTarget->Bind();
		mGraph->Render();
		mRenderTarget->ClearDepth();
		if (mCurrentGizmo != nullptr) {
			mGraph->RenderNodeBasic(mCurrentGizmo);
		}
		mRenderTarget->Release();


		ImGui::BeginChild("GameRender", ImVec2(0, 0), false, ImGuiWindowFlags_NoMove);
		// Get the size of the child (i.e. the whole draw size of the windows).
		ImVec2 wsize = ImGui::GetWindowSize();

		
		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		//ImGui::ColorButton("None", ImVec4(0, 0, 0, 1));
		ImGui::Image((ImTextureID)mRenderTarget->GetViewUI(), wsize, ImVec2(0, 0), ImVec2(1, 1));
			
		ImVec2 rsize = ImGui::GetItemRectSize();

		ImGui::EndChild();
		
	}


	ImGui::End();

	if (cam_interact) {

		int dx = Application::GetApp()->GetInput()->GetMouseDX();
		int dy = Application::GetApp()->GetInput()->GetMouseDY();

		cam_rotation.x -= dy;
		cam_rotation.y -= dx;

		mMainCamera->SetRotation(cam_rotation.x, cam_rotation.y, 0);
	}


	//update gizmos

	switch (mGizmoMode) {

	case GizmoMode::GizmoTranslate:

		mCurrentGizmo = mTranslateGizmo;

		break;
	case GizmoMode::GizmoRotate:

		break;
	case GizmoMode::GizmoScale:

		break;

	}

	if (mSelectedNode != nullptr) {

		mCurrentGizmo->SetPosition(mSelectedNode->GetPosition());

	}

	
}

// Content Browser

void ZenUI::MainContentBrowser() {

	if (!mCBF) {


		ImGui::SetNextWindowPos(mContentBrowserPos);
		ImGui::SetNextWindowSize(mContentBrowserSize);
		mCBF = true;
	}

	int flags = ImGuiWindowFlags_MenuBar;

	if (ImGui::Begin("Content Browser", &mContentBrowserOpen, flags))
	{



	}

	ImGui::End();

}

// Node Editor

void ZenUI::MainNodeEditor() {

	if (!mNEF)
	{
		ImGui::SetNextWindowPos(mNodeEditPos);
		ImGui::SetNextWindowSize(mNodeEditSize);
		mNEF = true;
	}

	int flags = ImGuiWindowFlags_MenuBar;

	if (ImGui::Begin("Node Editor"))
	{

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

//////////
void ZenUI::UpdateUI() {

	

}

void ZenUI::RenderUI() {

	MainWindow();

}

