#include "ZenUI.h"
#include "Importer.h"
#include "RayPicker.h"
#include "VString.h"
#include "DirCollection.h"
#include "VString.h"
#include "VFile.h"
#include "Application.h"

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



		//ImVec2 delta;


		mouse_delta.x = real_pos.x - prev_mouse.x;
		mouse_delta.y = real_pos.y - prev_mouse.y;

		prev_mouse = real_pos;

		if (real_pos.x > 0 && real_pos.y > 0 && real_pos.x < win_size.x && real_pos.y < win_size.y)
		{

			float spd = -0.15f;
			auto cam = mGraph->GetCamera();

			if (Application::GetApp()->GetInput()->IsMouseDown(1))
			{

				if (Application::GetApp()->GetInput()->IsKeyDown(KeyID::KeyW))
				{

					cam->Move(float3(0, 0, -spd));

				}
				if (Application::GetApp()->GetInput()->IsKeyDown(KeyID::KeyS)) {
					cam->Move(float3(0, 0, spd));
				}
				if (Application::GetApp()->GetInput()->IsKeyDown(KeyID::KeyA))
				{
					cam->Move(float3(spd, 0, 0));
				}
				if (Application::GetApp()->GetInput()->IsKeyDown(KeyID::KeyD))
				{
					cam->Move(float3(-spd, 0, 0));
				}

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
									mSelectedNode->Move(float3(((float)Application::GetApp()->GetInput()->GetMouseDX()) * mTranslateRatio, 0, 0));
										
								}
							}
							if (gLock_y) {

								if (mGizmoSpace == GizmoSpace::Global) {
									new_pos.y += ((float)-Application::GetApp()->GetInput()->GetMouseDY()) * mTranslateRatio;
									mSelectedNode->SetPosition(new_pos);
								}
								else {
									mSelectedNode->Move(float3(0,((float)Application::GetApp()->GetInput()->GetMouseDY())* mTranslateRatio, 0));
								}

							}
							if (gLock_z) {

								if (mGizmoSpace == GizmoSpace::Global) {
									new_pos.z += ((float)-Application::GetApp()->GetInput()->GetMouseDY()) * mTranslateRatio;
									mSelectedNode->SetPosition(new_pos);
								}
								else {
									mSelectedNode->Move(float3(0,0,((float)Application::GetApp()->GetInput()->GetMouseDY()) * mTranslateRatio));
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
									mSelectedNode->RotateGlobal(0, ((float)Application::GetApp()->GetInput()->GetMouseDX() * mRotateRatio), 0);
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
						auto result = mRayPick->MousePick((int)real_pos.x, (int)real_pos.y, (int)win_size.x, (int)win_size.y - 5, mMainCamera);

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

		if (mPPOn) {
			mPP->PreRender();
		}
		mGraph->RenderShadowMaps();
		mRenderTarget->Bind();
		//mGraph->Render();
		if (mPPOn) {
			mPP->Render();
		}
		else {
			mGraph->Render();
		}
		mRenderTarget->ClearDepth();
		auto cam = mGraph->GetCamera();
	//	cam->SetViewport(0, 0, win_size.x, win_size.y);
		mDraw->Begin();


		for (int i = 0; i < mGraph->LightCount(); i++) {
			Node3D* entity = mGraph->GetLight(i);

			float lx, ly;

			lx = 20;
			ly = 20;
			float4x4 model = entity->GetWorldMatrix();
			//angX = angX + 0.1f;

			// Camera is at (0, 0, -5) looking along the Z axis
			float4x4 View = cam->GetWorldMatrix().Inverse();;// float4x4::Translation(0.f, 0.0f, 5.0f);

			// Get pretransform matrix that rotates the scene according the surface orientation
			//auto SrfPreTransform = GetSurfacePretransformMatrix(float3{ 0, 0, 1 });

			// Get projection matrix adjusted to the current screen orientation
			auto Proj = cam->GetProjectionMatrix();  //float4x4::Projection( Maths::Deg2Rad(70.0f), 1024.0f / 760.0f, 0.001f, 1000.0f, false);

			// Compute world-view-projection matrix
			float4x4 m_WorldViewProjMatrix = model * View * Proj;
			m_WorldViewProjMatrix = m_WorldViewProjMatrix.Transpose();


			float4 pos = m_WorldViewProjMatrix * float4(0, 0, 0, 1.0);

			pos.x /= pos.w;
			pos.y /= pos.w;

			pos.x = (0.5 + pos.x * 0.5) * mRenderTarget->GetWidth();
			pos.y = (0.5 - pos.y * 0.5) * mRenderTarget->GetHeight();

			mDraw->DrawTexture(pos.x - 32, pos.y - 32, 64, 64, mSprLight, 1, 1, 1, 1, false);
			if (real_pos.x > pos.x - 32 && real_pos.x<pos.x + 32 && real_pos.y>pos.y - 32 && real_pos.y < pos.y + 32)
			{
				if (Application::GetApp()->GetInput()->IsMouseDown(0))
				{
					mSelectedNode = entity;
				}
			}
		}

		float4x4 pr = float4x4::OrthoOffCenter(0,mRenderTarget->GetWidth(),mRenderTarget->GetHeight(), 0, 0, 100.0f, false);


		mDraw->End(pr);
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
		else {
			mCurrentGizmo->SetRotation(0, 0, 0);
		}

		

	}

	float3 p1 = mCurrentGizmo->GetPosition();
	float3 p2 = mGraph->GetCamera()->GetPosition();
	float xd, yd, zd;
	xd = p2.x - p1.x;																							  
	yd = p2.y - p1.y;
	zd = p2.z - p2.z;
	float dis = sqrt(xd * xd + yd * yd + zd * zd);
	
	
	p1 = p2 - p1;
	
	
	dis = Diligent::length(p1);


	dis = dis / 1.0f;
	dis = dis * 0.1f;
	if (dis < 0.5f) dis = 0.5f;


	mCurrentGizmo->SetScale(float3(dis, dis, dis));

}

// Node Editor