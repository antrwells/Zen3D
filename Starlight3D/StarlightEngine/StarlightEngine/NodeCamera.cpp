#include "pch.h"
#include "NodeCamera.h"

//#include "Maths.h"
#include "Application.h"

	NodeCamera::NodeCamera() {

//		mProjectionMatrix = 
		//SetViewport(0, 0, Kinetic::App::Application::AppWidth, Kinetic::App::Application::AppHeight);


	}

	void NodeCamera::SetViewport(int x, int y, int w, int h) {

		//***

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

