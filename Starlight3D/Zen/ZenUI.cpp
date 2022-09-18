#include "ZenUI.h"
#include "UITheme_Neon.h"

ZenUI::ZenUI(SceneGraph* graph) {

	mGraph = graph;

	//Scene Graph
	mSceneGraphPos = ImVec2(0, 22);
	mSceneGraphSize = ImVec2(240, Application::GetApp()->GetHeight()-22-200);

	//Scene Viewport
	mSceneViewPos = ImVec2(240, 22);
	mSceneViewSize = ImVec2(750, Application::GetApp()->GetHeight() - 22-200);
	mRenderTarget = new RenderTarget2D(mSceneViewSize.x, mSceneViewSize.y);

	//Content Browser
	mContentBrowserPos = ImVec2(0, Application::GetApp()->GetHeight() - 200);
	mContentBrowserSize = ImVec2(Application::GetApp()->GetWidth(), 222);

	//Node Editor
	mNodeEditPos = ImVec2(990, 22);
	mNodeEditSize = ImVec2(Application::GetApp()->GetWidth() -990, Application::GetApp()->GetHeight() - 22 - 200);



}

void ZenUI::MainWindow() {


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

	int flags = ImGuiWindowFlags_MenuBar;

	if (ImGui::Begin("Scene Viewer", &mSceneViewOpen, flags)) {

		mGraph->RenderShadowMaps();
		mRenderTarget->Bind();
		mGraph->Render();
		mRenderTarget->Release();

		ImGui::BeginChild("GameRender");
		// Get the size of the child (i.e. the whole draw size of the windows).
		ImVec2 wsize = ImGui::GetWindowSize();
		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		ImGui::Image((ImTextureID)mRenderTarget->GetViewUI(), wsize, ImVec2(0, 0), ImVec2(1, 1));
		ImGui::EndChild();

	}


	ImGui::End();

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


//////////
void ZenUI::UpdateUI() {

	

}

void ZenUI::RenderUI() {

	MainWindow();

}

