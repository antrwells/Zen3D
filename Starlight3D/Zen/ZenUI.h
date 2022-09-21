#pragma once
#include "Application.h"
#include "UI.h"
#include "WindowControl.h"
#include "SceneGraph.h"

class RayPicker;

enum GizmoMode {

	GizmoTranslate, GizmoScale, GizmoRotate

};

class ZenUI
{
public:

	ZenUI(SceneGraph* graph);


	//Main UI
	void MainBGWindow();
	void MainWindow();
	void MainToolBar();
	void MainMenu();
	void MainViewPort();
	void MainContentBrowser();
	void MainNodeEditor();

	//SceneGraph related - 
	void SceneGraphWindow();
	void SceneTree(Node3D* node);

	int GetWidth() {
		
		return Application::GetApp()->GetWidth();

	}
	int GetHeight() {

		return Application::GetApp()->GetHeight();

	}

	void UpdateUI();
	void RenderUI();

private:


	// Open States
	bool mMainOpen = true;
	bool mSceneGraphOpen = true;
	bool mSceneViewOpen = true;
	bool mContentBrowserOpen = true;
	bool mNodeEditorOpen = true;

	//Raw Data
	SceneGraph* mGraph;


	//Positions & Size
	//SceneGraph
	ImVec2 mSceneGraphPos;
	ImVec2 mSceneGraphSize;
	bool mSGF = false;
	Node3D* mSelectedNode = nullptr;

	//Main Viewport
	ImVec2 mSceneViewPos;
	ImVec2 mSceneViewSize;
	RenderTarget2D* mRenderTarget;
	bool mSVF = false;
	NodeCamera* mMainCamera;
	ImVec2 prev_mouse;
	ImVec2 cam_rotation;
	ImVec2 mouse_delta;
	bool cam_interact = false;

	//Main Viewport Resources
	NodeEntity* mTranslateGizmo;
	NodeEntity* mRotateGizmo;
	NodeEntity* mScaleGizmo;
	NodeEntity* mCurrentGizmo;

	//Main Viewport Globals
	bool gLock_x, gLock_y, gLock_z;
	bool gLock = false;
	GizmoMode mGizmoMode;

	//Main viewport vars
	float mTranslateRatio = 0.02f;


	//Content Browser
	ImVec2 mContentBrowserPos;
	ImVec2 mContentBrowserSize;
	bool mCBF = false;

	//Node Editor
	ImVec2 mNodeEditPos;
	ImVec2 mNodeEditSize;
	bool mNEF = false;


	// Ray Picking
	RayPicker* mRayPick = nullptr;


	

};


