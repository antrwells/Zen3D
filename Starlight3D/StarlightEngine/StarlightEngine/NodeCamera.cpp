#include "pch.h"
#include "NodeCamera.h"

#include "Maths.h"
#include "Application.h"
#include "Texture2D.h"

	NodeCamera::NodeCamera() {

//		mProjectionMatrix = 
		SetViewport(0, 0,Application::GetApp()->GetWidth(),Application::GetApp()->GetHeight());
		mType = NodeType::Camera;
		mName = "Game Camera";
		if (mLensDefualt == nullptr) {
			mLensDefualt = new Texture2D("data/lensImage.png");
		}
		mLensImage = mLensDefualt;
	}

	Texture2D* NodeCamera::mLensDefualt = nullptr;

	void NodeCamera::SetViewport(int x, int y, int w, int h) {

		//***

		mProjectionMatrix = float4x4::Projection(Maths::Deg2Rad(mFOV), (float)(w) / (float)(h),mMinZ,mMaxZ,false);


		//float nv = Kinetic::Maths::Deg2Rad(mFOV);
		//mProjectionMatrix = glm::perspective<float>(nv, (float)w / (float)h, mMinZ, mMaxZ);
		mWidth = w;
		mHeight = h;
		//mProj = glm::perspectiveFov(FOV, (float)w, (float)h, minZ, maxZ);


	}


	float4x4 NodeCamera::GetWorldMatrix() {

		float4x4 world_matrix =  Node3D::GetWorldMatrix();

		return world_matrix.Inverse();

	}

	float4x4 NodeCamera::GetProjectionMatrix() {

		return mProjectionMatrix;

	}

