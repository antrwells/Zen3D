#include "ZenUI.h"
#include "UITheme_Neon.h"
#include "Importer.h"
#include "RayPicker.h"
#include "VString.h"
#include "DirCollection.h"
#include "VString.h"
#include "VFile.h"


ZenUI* ZenUI::mUI = nullptr;

SceneGraph* ZenUI::GetGraph() {

	return mGraph;

}

void OtherTheme() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 5.3f;
	style.FrameRounding = 2.3f;
	style.ScrollbarRounding = 0;

	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
	//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
	//style.Colors[ImGuiCol_ComboBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
//	style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
//	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
//	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	//style.Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
	//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

}

void CoolTheme(bool dark) {
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	style.Alpha = 1.0f;
	style.FrameRounding = 3.0f;
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
	//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	//style.Colors[ImGuiCol_ComboBg] = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	//style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	//style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
//	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	//style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
	//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
//	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	float alpha_ = 0.95f;
	if (dark)
	{
		for (int i = 0; i <= ImGuiCol_COUNT; i++)
		{
			ImVec4& col = style.Colors[i];
			float H, S, V;
			ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

			if (S < 0.1f)
			{
				V = 1.0f - V;
			}
			ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
			if (col.w < 1.00f)
			{
				col.w *= alpha_;
			}
		}
	}
	else
	{
		for (int i = 0; i <= ImGuiCol_COUNT; i++)
		{
			ImVec4& col = style.Colors[i];
			if (col.w < 1.00f)
			{
				col.x *= alpha_;
				col.y *= alpha_;
				col.z *= alpha_;
				col.w *= alpha_;
			}
		}
	}
}

void LightBlueTheme() {

	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	style.Colors[ImGuiCol_Text] = ImColor(220, 220, 220);
	style.Colors[ImGuiCol_WindowBg] = ImColor(25, 55, 75);
	style.Colors[ImGuiCol_Border] = ImColor(0, 70, 70);
	style.Colors[ImGuiCol_Button] = ImColor(0, 94, 94);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(0, 130,130);
	style.Colors[ImGuiCol_FrameBg] = ImColor(0, 92,92);
	style.Colors[ImGuiCol_MenuBarBg] = ImColor(0, 25, 25);
	style.Colors[ImGuiCol_PopupBg] = ImColor(0, 128, 128);
	style.Colors[ImGuiCol_ScrollbarBg] = ImColor(0, 52, 52);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(0, 104, 104);
	style.Colors[ImGuiCol_TitleBg] = ImColor(0, 52, 52);
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(0, 94, 94);
	style.Alpha = 1;
	style.AntiAliasedFill = true;
	style.AntiAliasedLines = true;
	style.WindowBorderSize = 5.0f;
	style.WindowTitleAlign = ImVec2(0.5, 0.5);
	style.FrameRounding = 0.25f;
	style.FrameBorderSize = 0.0f;

}

void ReallyDarkTheme() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	style.Colors[ImGuiCol_Text] = ImColor(220, 220, 220);
	style.Colors[ImGuiCol_WindowBg] = ImColor(25, 25, 25);
	style.Colors[ImGuiCol_Border] = ImColor(30, 30, 30);
	style.Colors[ImGuiCol_Button] = ImColor(54, 54, 54);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(76, 76, 76);
	style.Colors[ImGuiCol_FrameBg] = ImColor(80, 82, 82);
	style.Colors[ImGuiCol_MenuBarBg] = ImColor(32, 32, 32);
	style.Colors[ImGuiCol_PopupBg] = ImColor(128, 128, 128);
	style.Colors[ImGuiCol_ScrollbarBg] = ImColor(0, 32, 32);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(64, 64, 64);
	style.Colors[ImGuiCol_TitleBg] = ImColor(12, 12, 12);
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(64, 64, 64);

	style.FrameRounding = 0.f;
	style.FrameBorderSize = 0.5f;
	style.Alpha = 1;
	style.AntiAliasedFill = true;
	style.AntiAliasedLines = true;
	style.WindowBorderSize = 5.0f;
	style.WindowTitleAlign = ImVec2(0.5, 0.5);
	style.FrameRounding = 0.25f;
	style.FrameBorderSize = 0.0f;
}

void BrightTheme() {
	// Setup style
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.31f, 0.25f, 0.24f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.74f, 0.74f, 0.94f, 1.00f);
	//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.68f, 0.68f, 0.68f, 0.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.62f, 0.70f, 0.72f, 0.56f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.33f, 0.14f, 0.47f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.97f, 0.31f, 0.13f, 0.81f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.42f, 0.75f, 1.00f, 0.53f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.65f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.62f, 0.80f, 0.15f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.39f, 0.64f, 0.80f, 0.30f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.28f, 0.67f, 0.80f, 0.59f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.48f, 0.53f, 0.67f);
	//style.Colors[ImGuiCol_ComboBg] = ImVec4(0.89f, 0.98f, 1.00f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.48f, 0.47f, 0.47f, 0.71f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.47f, 0.99f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.79f, 0.18f, 0.78f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.42f, 0.82f, 1.00f, 0.81f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.72f, 1.00f, 1.00f, 0.86f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.65f, 0.78f, 0.84f, 0.80f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.88f, 0.94f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.68f, 0.74f, 0.80f);//ImVec4(0.46f, 0.84f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.60f, 0.80f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	//style.Colors[ImGuiCol_CloseButton] = ImVec4(0.41f, 0.75f, 0.98f, 0.50f);
	//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 0.47f, 0.41f, 0.60f);
	//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 0.16f, 0.00f, 1.00f);
	//style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.99f, 0.54f, 0.43f);
//	style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.82f, 0.92f, 1.00f, 0.90f);
	style.Alpha = 1.0f;
//	style.WindowFillAlphaDefault = 1.0f;
	style.FrameRounding = 4;
	style.IndentSpacing = 12.0f;
}

void ZenUI::CreateUI(SceneGraph* graph) {
	//set up theme
	mUI = this;
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	style.Colors[ImGuiCol_Text] = ImColor(220, 220, 220);
	style.Colors[ImGuiCol_WindowBg] = ImColor(5, 5, 5);

	style.FrameRounding = 0.f;
	style.FrameBorderSize = 0.5f;
	//ImGui::StyleColorsDark();
	//ReallyDarkTheme();
	//OtherTheme()
	CoolTheme(false);

	//	BrightTheme();

	//LightBlueTheme();


	//
	mScriptNameBuf = (char*)malloc(512);
	mScriptNameBuf[0] = "\0"[0];

	auto importer = new Importer;

	mGraph = graph;

	mMainCamera = graph->GetCamera();
	//mEditCam = mMainCamera;
	//mGameCam = new NodeCamera;
	//mGraph->AddCamera(mGameCam);
	mCurrentCamera = mMainCamera;

	graph->AddNode(mMainCamera);
	mEditCam = mMainCamera;

	//Toolbar
	mToolbarPos = ImVec2(0, 22);
	mToolbarSize = ImVec2(Application::GetApp()->GetWidth(), 54);

	//Icons

	mIconTranslate = (Texture2D*)pUI->GetResource("MoveIcon");
	mIconRotate = (Texture2D*)pUI->GetResource("RotateIcon"); // new Texture2D("edit/icon/rotateIcon.png");
	mIconScale = (Texture2D*)pUI->GetResource("ScaleIcon"); //;new Texture2D("edit/icon/scaleicon.png");
	mIconPlay = (Texture2D*)pUI->GetResource("PlayIcon");// new Texture2D("edit/icon/playicon.png");
	mIconStop = (Texture2D*)pUI->GetResource("StopIcon");// n ew Texture2D("edit/icon/stopicon.png");

	mWhiteTex = (Texture2D*)pUI->GetResource("WhiteTex");

	//Scene Graph
	mSceneGraphPos = ImVec2(0, mToolbarSize.y + 12);
	mSceneGraphSize = ImVec2(240, Application::GetApp()->GetHeight() - 22 - 200 - mToolbarSize.y);

	//Scene Viewport Resources
	mSceneViewPos = ImVec2(240, mToolbarSize.y + 12);
	mSceneViewSize = ImVec2(750, Application::GetApp()->GetHeight() - 22 - 200 - mToolbarSize.y);
	mRenderTarget = new RenderTarget2D(mSceneViewSize.x, mSceneViewSize.y);
	cam_rotation = ImVec2(0, 0);
	mTranslateGizmo = (NodeEntity*)pUI->GetResource("GizTranslate");//  importer->ImportAI("edit/gizmo/translate1.fbx");
	mRotateGizmo = (NodeEntity*)pUI->GetResource("GizRotate");//importer->ImportAI("edit/gizmo/rotate1.fbx");
	mScaleGizmo = (NodeEntity*)pUI->GetResource("GizScale");//importer->ImportAI("edit/gizmo/scale1.fbx");
	mCamGizmo = (NodeEntity*)pUI->GetResource("GizCam");
	auto mesh = mCamGizmo->GetMesh(0);
	auto verts = mesh->GetVertices();

	for (int i = 0; i < verts.size(); i++) {

		auto vert = verts[i];
		vert.color = float4(1, 1, 1, 0.25f);
		verts[i] = vert;

	}
	mesh->SetVertices(verts);
	mesh->CreateBuffers();

	int ba = 5;
	mTranslateGizmo->GetMesh(0)->SetName("Z");
	mTranslateGizmo->GetMesh(1)->SetName("X");
	mTranslateGizmo->GetMesh(2)->SetName("Y");
	mRotateGizmo->GetMesh(2)->SetName("X");
	mRotateGizmo->GetMesh(0)->SetName("Y");
	mRotateGizmo->GetMesh(1)->SetName("Z");
	mScaleGizmo->GetMesh(0)->SetName("X");
	mScaleGizmo->GetMesh(1)->SetName("Y");
	mScaleGizmo->GetMesh(2)->SetName("Z");
	auto red_tex = (Texture2D*)pUI->GetResource("Red");
	auto blue_tex = (Texture2D*)pUI->GetResource("Green");// new Texture2D("edit/gizmo/blue.png");
	auto green_tex = (Texture2D*)pUI->GetResource("Blue"); //new Texture2D("edit/gizmo/green.png");

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
	mContentBrowserPos = ImVec2(0, 12 + Application::GetApp()->GetHeight() - 200 - 22);
	mContentBrowserSize = ImVec2(Application::GetApp()->GetWidth(), 222 - 12);// mToolbarSize.y);

	//Icons

	mIconFolder = (Texture2D*)pUI->GetResource("FolderIcon");// new Texture2D("edit/browser/foldericon.png");
	mIconFile = (Texture2D*)pUI->GetResource("FileIcon");//  new Texture2D("edit/browser/fileicon.png");
	mIconScript = (Texture2D*)pUI->GetResource("ScriptIcon");//  new Texture2D("edit/browser/scripticon.png");

	//Node Editor
	mNodeEditPos = ImVec2(990, mToolbarSize.y + 12);
	mNodeEditSize = ImVec2(Application::GetApp()->GetWidth() - 990, Application::GetApp()->GetHeight() - 22 - 200 - mToolbarSize.y);


	//Other Resources

	mRayPick = new RayPicker(mGraph);

	mToolbarSize.y = mToolbarSize.y - 8;

	//Sprites

	mDraw = new SmartDraw(Application::GetApp());

	mSprLight = (Texture2D*)pUI->GetResource("LightIcon");//  new Texture2D("edit/sprites/light1.png", true);
	mSprCam = (Texture2D*)pUI->GetResource("CameraIcon");
	mSprNode = (Texture2D*)pUI->GetResource("NodeIcon");

	//Post Processing

	mPP = new PostProcessing(mGraph);
	mPPBloom = new PPBloom();
	mPPOutline = new PPOutline();
	mPP->AddPostProcess(mPPOutline);

	mEditGraph = new SceneGraph;
	ScanContent("c:/ZenContent/");

	Notify("Zen3D started", "You may now begin to edit your project.");

	//ImFont* font1 = io.Fonts->AddFontDefault();
	

}

ZenUI::ZenUI() {



	pUI = new FSPayload("UIPayload");
	pUI->AddFile("MoveIcon", "edit/icon/MoveIcon2.png", ResourceType::TextureFlat);
	pUI->AddFile("RotateIcon", "edit/icon/rotateIcon.png", ResourceType::TextureFlat);
	pUI->AddFile("ScaleIcon", "edit/icon/scaleicon.png", ResourceType::TextureFlat);
	pUI->AddFile("PlayIcon", "edit/icon/playicon.png", ResourceType::TextureFlat);
	pUI->AddFile("StopIcon", "edit/icon/stopicon.png", ResourceType::TextureFlat);
	pUI->AddFile("Red", "edit/gizmo/red.png", ResourceType::TextureFlat);
	pUI->AddFile("Green", "edit/gizmo/green.png", ResourceType::TextureFlat);
	pUI->AddFile("Blue", "edit/gizmo/blue.png", ResourceType::TextureFlat);
	pUI->AddFile("FolderIcon", "edit/browser/foldericon.png", ResourceType::TextureFlat);
	pUI->AddFile("FileIcon", "edit/browser/fileicon.png",ResourceType::TextureFlat);
	pUI->AddFile("ScriptIcon", "edit/browser/scripticon.png", ResourceType::TextureFlat);
	pUI->AddFile("LightIcon", "edit/sprites/light1.png", ResourceType::TextureFlat);
	pUI->AddFile("CameraIcon", "edit/sprites/camera1.png", ResourceType::TextureFlat);
	pUI->AddFile("NodeIcon", "edit/sprites/emptynode1.png", ResourceType::TextureFlat);
	pUI->AddFile("GizTranslate", "edit/gizmo/translate1.fbx", ResourceType::ModelProp);
	pUI->AddFile("GizRotate", "edit/gizmo/rotate1.fbx", ResourceType::ModelProp);
	pUI->AddFile("GizScale", "edit/gizmo/scale1.fbx", ResourceType::ModelProp);
	pUI->AddFile("GizCam", "edit/gizmo/cam1.fbx", ResourceType::ModelProp);
	pUI->AddFile("WhiteTex", "edit/white.png", ResourceType::TextureFlat);
	//pUI->LoadThreaded();

	mLMRes[0] = 512;
	mLMRes[1] = 512;
	

}

void ZenUI::MainWindow() {

	MainBGWindow();

	MainMenu();

	SceneGraphWindow();

	MainViewPort();

	MainContentBrowser();

	MainNodeEditor();

	PostProcessWindow();

	LightMapperWindow();

	if (mSaveSceneOpen) {

		SaveSceneDialog();

	}

	LoadingIndicator();

	Notifications();



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

