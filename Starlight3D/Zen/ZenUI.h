#pragma once
#include "Application.h"
#include "UI.h"
#include "WindowControl.h"
#include "SceneGraph.h"
#include "VString.h"
#include "VFile.h"
#include "DirCollection.h"
class RayPicker;
//class DirCollection;

enum GizmoMode {

	GizmoTranslate, GizmoScale, GizmoRotate

};

enum GizmoSpace {
	Local,Global,Smart
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

	//Content Browser Related
	void ScanContent(std::string path);


	int GetWidth() {
		
		return Application::GetApp()->GetWidth();

	}
	int GetHeight() {

		return Application::GetApp()->GetHeight();

	}

	void UpdateUI();
	void RenderUI();

	//Other functionality.
	Node3D* ImportNode(const char* path);

	// Other Temporary UI

	void GetScriptName();

	//Game Related
	void BeginPlay();
	void EndPlay();

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
	
	
	//ToolBar
	ImVec2 mToolbarPos;
	ImVec2 mToolbarSize;
	int menuBarHeight;
	int mSpaceItem = 0;

	//Images
	Texture2D* mIconTranslate, * mIconRotate, * mIconScale;
	Texture2D* mIconPlay, * mIconStop, * mIconPause;

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
	float mRotateRatio = 0.09f;
	float mScaleRatio = 0.02f;
	bool first_render = true;
	GizmoSpace mGizmoSpace;

	//Content Browser
	ImVec2 mContentBrowserPos;
	ImVec2 mContentBrowserSize;
	bool mCBF = false;
	bool mGetScriptName = false;
	char* mScriptNameBuf;

	Texture2D* mIconFile, * mIconFolder, * mIconScript;
	DirCollection* mDir;
	std::vector<DirCollection*> mDirStack;

	//Node Editor
	ImVec2 mNodeEditPos;
	ImVec2 mNodeEditSize;
	bool mNEF = false;

	bool Dragging = false;
	DirEntry* mDragEntry = nullptr;
	DirEntry mDragEntryRef;

	// Ray Picking
	RayPicker* mRayPick = nullptr;

	//Paths and such.
	VString* mContentPath;
	

	//Play 
	bool mPlaying = false;

};


