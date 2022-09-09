#include "pch.h"
#include "CubeRenderer.h"


CubeRenderer::CubeRenderer(SceneGraph* graph, RenderTargetCube* cube)
{

	mGraph = graph;
	mRT = cube;

}

void CubeRenderer::RenderDepth(float3 pos,float maxZ) {

	auto prev_cam = mGraph->GetCamera();

	auto new_cam = new NodeCamera;

	mGraph->SetCamera(new_cam);

	new_cam->SetPosition(pos);

	new_cam->SetRotation(-20, 90, 0);
	new_cam->SetMinZ(prev_cam->GetMinZ());
	new_cam->SetMaxZ(maxZ);
	new_cam->SetFOV(90);
	new_cam->SetViewport(0, 0, mRT->GetWidth(), mRT->GetHeight());


	for (int i = 0;i < 6;i++) {

		switch (i) {
		case 1:
			new_cam->SetRotation(float4x4::RotationY(+PI_F / 2.f));
			break;
		case 0:
			new_cam->SetRotation(float4x4::RotationY(-PI_F / 2.f));

			break;
		case 3:
			new_cam->SetRotation(float4x4::RotationX(-PI_F / 2.f));
			break;
		case 2:
			new_cam->SetRotation(float4x4::RotationX(+PI_F / 2.f));
			break;
		case 4:
			new_cam->SetRotation(float4x4::Identity());
			break;
		case 5:
			new_cam->SetRotation(float4x4::RotationY(PI_F));
			break;
		}
		mRT->Bind(i);

		mGraph->RenderDepth();

		mRT->Release(i);

	}

	mGraph->SetCamera(prev_cam);

}

void CubeRenderer::Render(float3 pos) {

	auto prev_cam = mGraph->GetCamera();

	auto new_cam = new NodeCamera;

	mGraph->SetCamera(new_cam);

	new_cam->SetPosition(pos);

	new_cam->SetRotation(-20, 90, 0);
	new_cam->SetFOV(90);
	new_cam->SetViewport(0, 0, mRT->GetWidth(), mRT->GetHeight());

	for (int i = 0;i < 6;i++) {

		switch (i) {
		case 1:
			new_cam->SetRotation(float4x4::RotationY(+PI_F / 2.f));
			break;
		case 0:
			new_cam->SetRotation(float4x4::RotationY(-PI_F / 2.f));

			break;
		case 3:
			new_cam->SetRotation(float4x4::RotationX(-PI_F / 2.f));
			break;
		case 2:
			new_cam->SetRotation(float4x4::RotationX(+PI_F / 2.f));
			break;
		case 4:
			new_cam->SetRotation(float4x4::Identity());
			break;
		case 5:
			new_cam->SetRotation(float4x4::RotationY(PI_F));
			break;
		}

		mRT->Bind(i);

		mGraph->Render();

		mRT->Release(i);

	}

	mGraph->SetCamera(prev_cam);

}

void CubeRenderer::SetRenderTargetCube(RenderTargetCube* cube) {

	mTexCube = new TextureCube(cube);
	mRT = cube;
	

}

TextureCube* CubeRenderer::GetTextureCube() {

	if (mTexCube == nullptr) {

		mTexCube = new TextureCube(mRT);

	}

	return mTexCube;

}