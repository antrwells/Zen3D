#pragma once
#include "Application.h"

#include "WindowControl.h"
#include "SceneGraph.h"
#include "VString.h"
#include "VFile.h"
#include "DirCollection.h"
#include "FSPayload.h"
#include "PostProcessing.h"
#include "PPBloom.h"
#include "PPOutline.h"
#include "ZNotification.h"
#include <stack>
#include "imfilebrowser.h"
class RayPicker;

//class DirCollection;

enum GizmoMode {

	GizmoTranslate, GizmoScale, GizmoRotate

};

enum GizmoSpace {
	Local,Global,Smart
};

struct NodeList {

	std::string type;
	std::vector<std::string> names;
	std::vector<std::string> paths;

};

class ZenUI
{
public:




	ZenUI();

	void CreateUI(SceneGraph* graph);
	FSPayload* GetPayload()
	{
		return pUI;
	}
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


	//Menu/Optional
	
	//PostProcessing
	void PostProcessWindow();

	//Lightmapper
	void LightMapperWindow();


	//Material Editor
	void MaterialEditor();

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

	//fonts
	void CheckFont() {
		ImGuiIO& io = ImGui::GetIO();
		if (bigFont == nullptr) {

		//	bigFont = io.Fonts->AddFontFromFileTTF("edit/fonts/bigfont.ttf", 32.0f);

		}

	}

	//Notifications

	void Notify(std::string title, std::string msg);
	void Notifications();

	// Loading
	bool LoadPayload(FSPayload* payload, void(*mWhenDone)(FSPayload* pl));
	float PayloadPercent();
		void LoadingIndicator();

	//Scene IO
		void OpenSaveScene()
		{
			mSaveSceneOpen = true;
		}
		void OpenSaveNode() {
			mSaveNodeOpen = true;
		}

		void SaveSceneDialog();
		void SaveNodeDialog();

		void LoadScene(const char* path);
		void SaveScene(const char* path);
		void SaveNode(const char* path);
		void LoadNode(const char* path);

	//globals

	static ZenUI* mUI;

	SceneGraph* GetGraph();

	//Renderer - A combo box that sets up all the types of rendering possible.
	void SetProjectPath(std::string path) {
		mProjectPath = path;
	}

	//Other Editors
	void EditMaterial();

	void AddNodeAndScript(std::string path,std::string name) {

		NodeEntity* node = new NodeEntity;
		node->AddScript(path, name);
		node->SetName(VString(name.c_str()).GetConst());
		mGraph->AddNode(node);


	}

private:

	std::string mProjectPath = "";
	// Open States
	bool mMainOpen = true;
	bool mSceneGraphOpen = true;
	bool mSceneViewOpen = true;
	bool mContentBrowserOpen = true;
	bool mNodeEditorOpen = true;
	bool mSaveSceneOpen = false;
	bool mSaveNodeOpen = false;
	//Renderer
	bool mPostProcessOpen = false;
	bool mLightmapperOpen = false;
	bool mPPFirst = true;
	bool mLMFirst = true;
	bool mEditMatFirst = true;
	bool mPPOn = false;

	//Raw Data
	SceneGraph* mGraph;


	//Positions & Size
	
	SceneGraph* mGameGraph = nullptr;
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
	Node3D* mEditNode = nullptr;

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
	NodeEntity* mCamGizmo;

	//Camera
	NodeCamera* mCurrentCamera = nullptr;
	int mCurrentCamID = 0;
	NodeCamera* mEditCam = nullptr;

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

	//PP
	PostProcessing* mPP = nullptr;
	PPBloom* mPPBloom = nullptr;
	PPOutline* mPPOutline = nullptr;

	//Lightmapper
	int mLMRes[2];

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


	//Sprites
	SmartDraw* mDraw;
	Texture2D* mSprLight;
	Texture2D* mSprCam;
	Texture2D* mSprNode;

	//Payloads
	FSPayload* pUI;


	//Fonts
	ImFont* bigFont = nullptr;


	//Notifications

	std::vector<ZNotification*> mNotices;
	ZNotification* mCurrentNotice = nullptr;

	//Loading
	FSPayload* mCurrentPayload = nullptr;
	void(*mWhenDone)(FSPayload* pl);// mWhenDone;

	//saving
	char* saveName = nullptr;

	Texture2D* mWhiteTex;


	//Clipboard
	Node3D* mClipNode = nullptr;

	bool BlockSG = false;

	SceneGraph* mEditGraph;


	ImGui::FileBrowser fileBrowse;

	bool fileBrowseOpen = false;
	//Misc
	int empty_index = 0;
	
	//Other Editors
	Material* mEditMaterial = nullptr;

	//Editor Grid
	MeshLines* mEditGrid;
	bool mGridOn = true;

	//Built in nodes.
	std::vector<NodeList> nodeLists;

};



