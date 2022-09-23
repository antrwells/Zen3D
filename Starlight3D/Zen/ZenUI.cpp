#include "ZenUI.h"
#include "UITheme_Neon.h"
#include "Importer.h"
#include "RayPicker.h"
#include "VString.h"

ZenUI::ZenUI(SceneGraph* graph) {


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
	mGizmoMode = GizmoMode::GizmoScale;
	mGizmoSpace = GizmoSpace::Global;


	//mGraph->AddNode(mTranslateGizmo);
	mTranslateGizmo->SetPosition(float3(0, 1, 0));
	mRotateGizmo->SetPosition(float3(0, 1, 0));
	mScaleGizmo->SetPosition(float3(0, 1, 0));


	//Content Browser
	mContentBrowserPos = ImVec2(0, 12+Application::GetApp()->GetHeight() - 200-22);
	mContentBrowserSize = ImVec2(Application::GetApp()->GetWidth(), 222-12);// mToolbarSize.y);

	//Node Editor
	mNodeEditPos = ImVec2(990,mToolbarSize.y+12);
	mNodeEditSize = ImVec2(Application::GetApp()->GetWidth() -990, Application::GetApp()->GetHeight() - 22 - 200-mToolbarSize.y);


	//Other Resources

	mRayPick = new RayPicker(mGraph);

	mToolbarSize.y = mToolbarSize.y - 8;


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
	//	printf("S>>>");
	//	printf(node->GetName());
		//printf("\n");

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
	ImGui::Button("Check");

	ImGui::End();

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

		win_size.y = win_size.y - 32;
		win_size.x -= 15;
		win_size.y -= 6;

		if ((int)(win_size.x) != mRenderTarget->GetWidth() || (int)(win_size.y) != mRenderTarget->GetHeight() || first_render) {

			mRenderTarget = new RenderTarget2D((int)win_size.x, (int)win_size.y);
			printf("============================================================>\n");
			auto cam = mGraph->GetCamera();
			cam->SetViewport(0, 0, win_size.x, win_size.y);
			first_render = false;

		}

		ImVec2 real_pos;
		real_pos.x = mp.x - win_pos.x;
		real_pos.y = mp.y - win_pos.y;
		//real_pos.x = real_pos.x - 4;
		real_pos.y = real_pos.y - 29;
		real_pos.x = real_pos.x - 7;


		printf("Mouse x:%d Mouse y:%d\n", (int)real_pos.x, (int)real_pos.y);
		printf("Win SizeX:%d Win Size Y:%d\n", (int)win_size.x, (int)win_size.y);


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
						float3 new_scale = mSelectedNode->GetScale();

						switch (mGizmoMode) {

						case GizmoMode::GizmoTranslate:
						

							if (gLock_x)
							{

								if (mGizmoSpace == GizmoSpace::Global) {
									new_pos.x += ((float)Application::GetApp()->GetInput()->GetMouseDX()) * mTranslateRatio;
									mSelectedNode->SetPosition(new_pos);
								}
								else {

								}
							}
							if (gLock_y) {

								if (mGizmoSpace == GizmoSpace::Global) {
									new_pos.y += ((float)-Application::GetApp()->GetInput()->GetMouseDY()) * mTranslateRatio;
									mSelectedNode->SetPosition(new_pos);
								}
								else {

								}

							}
							if (gLock_z) {

								if (mGizmoSpace == GizmoSpace::Global) {
									new_pos.z += ((float)-Application::GetApp()->GetInput()->GetMouseDY()) * mTranslateRatio;
									mSelectedNode->SetPosition(new_pos);
								}
								else {

								}

							}
							break;
						case GizmoMode::GizmoRotate:

							if (gLock_x) {

								if (mGizmoSpace == GizmoSpace::Local) {
									mSelectedNode->RotateLocal(((float)Application::GetApp()->GetInput()->GetMouseDX() * mRotateRatio), 0, 0);
								}
								else {
									mSelectedNode->RotateGlobal(((float)Application::GetApp()->GetInput()->GetMouseDX() * mRotateRatio), 0, 0);
								}
							}

							if (gLock_y) {

								if (mGizmoSpace == GizmoSpace::Local) {
									mSelectedNode->RotateLocal(0, ((float)Application::GetApp()->GetInput()->GetMouseDX() * mRotateRatio), 0);
								}
								else {
									mSelectedNode->RotateGlobal(0, ((float)Application::GetApp()->GetInput()->GetMouseDX()* mRotateRatio), 0);
								}
							}

							if (gLock_z) {

								if (mGizmoSpace == GizmoSpace::Local) {
									mSelectedNode->RotateLocal(0, 0, -((float)Application::GetApp()->GetInput()->GetMouseDX() * mRotateRatio));
								}
								else {
									mSelectedNode->RotateGlobal(0, 0, -((float)Application::GetApp()->GetInput()->GetMouseDX() * mRotateRatio));
								}

							}

							break;
						case GizmoMode::GizmoScale:


								if (gLock_x)
								{

									
										new_scale.x += ((float)Application::GetApp()->GetInput()->GetMouseDX()) * mScaleRatio;
										mSelectedNode->SetScale(new_scale);
									
								}
								if (gLock_y) {

							
										new_scale.y += ((float)-Application::GetApp()->GetInput()->GetMouseDY()) * mScaleRatio;
										mSelectedNode->SetScale(new_scale);
									

								}
								if (gLock_z) {

									
										new_scale.z += ((float)-Application::GetApp()->GetInput()->GetMouseDY()) * mScaleRatio;
										mSelectedNode->SetScale(new_scale);
									
								}

								break;
						}


					}

				}
				else {
					bool check_rest = true;
					if (mSelectedNode != nullptr) {
						auto giz_result = mRayPick->MousePickNode((int)real_pos.x, (int)real_pos.y, (int)win_size.x, (int)win_size.y, mCurrentGizmo, mMainCamera);

						int bbb = 5;

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
						auto result = mRayPick->MousePick((int)real_pos.x, (int)real_pos.y, (int)win_size.x, (int)win_size.y-5, mMainCamera);

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

		mCurrentGizmo = mRotateGizmo;

		break;
	case GizmoMode::GizmoScale:

		mCurrentGizmo = mScaleGizmo;

		break;

	}

	if (mSelectedNode != nullptr) {

		mCurrentGizmo->SetPosition(mSelectedNode->GetPosition());
		if (mGizmoSpace == GizmoSpace::Local) {

			mCurrentGizmo->SetRotation4x4(mSelectedNode->GetRotation4x4());

		}
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
	 
	MainToolBar();
	MainWindow();

}

