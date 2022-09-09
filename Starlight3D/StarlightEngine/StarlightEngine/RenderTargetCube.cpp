#include "pch.h"
#include "RenderTargetCube.h"
#include "Application.h"

RenderTargetCube::RenderTargetCube(int width, int height) {
	mWidth = width;
	mHeight = height;
	mClearColor = float4(1, 1, 1, 1);
	//color
	TextureDesc RTColorDesc;
	RTColorDesc.Name = "RenderTargetCube Texture";
	RTColorDesc.Type = RESOURCE_DIM_TEX_CUBE;
	RTColorDesc.Width = width;// pSwapChain->GetDesc().Width;
	RTColorDesc.Height = height;// pSwapChain->GetDesc().Height;
	RTColorDesc.MipLevels = 1;
	RTColorDesc.ArraySize = 6;
	RTColorDesc.Usage = USAGE_DEFAULT;
	
	RTColorDesc.Format = TEX_FORMAT_RGBA32_FLOAT;
	RTColorDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
	RTColorDesc.ClearValue.Format = RTColorDesc.Format;
	RTColorDesc.ClearValue.Color[0] = 0.650f;
	RTColorDesc.ClearValue.Color[1] = 0.150f;
	RTColorDesc.ClearValue.Color[2] = 0.150f;
	RTColorDesc.ClearValue.Color[3] = 1.f;
	Application::GetApp()->GetDevice()->CreateTexture(RTColorDesc, nullptr, &pRTColor);
	m_pColorRTV = pRTColor->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

	for (int i = 0;i < 6;i++) {

		TextureViewDesc RTVDesc{ "TexCube Bind - FACE", TEXTURE_VIEW_RENDER_TARGET, RESOURCE_DIM_TEX_2D_ARRAY };
		RTVDesc.MostDetailedMip = 0;
		RTVDesc.FirstArraySlice = i;
		RTVDesc.NumArraySlices = 1;
		RefCntAutoPtr<ITextureView> pRTV;
		pRTColor->CreateView(RTVDesc, &pRTV);
		FaceView[i] = pRTV;

	}

	//depth
	TextureDesc RTDepthDesc;
	RTDepthDesc.Name = "Cubfffe depth buffer";
	RTDepthDesc.Format = TEX_FORMAT_D32_FLOAT;
	RTDepthDesc.BindFlags = BIND_DEPTH_STENCIL;
	// Define optimal clear value
	RTDepthDesc.ClearValue.Format = RTDepthDesc.Format;
	RTDepthDesc.ClearValue.DepthStencil.Depth = 1;
	RTDepthDesc.ClearValue.DepthStencil.Stencil = 0;
	RTDepthDesc.Type = RESOURCE_DIM_TEX_2D;
//	RTDepthDesc.MipLevels = 1;
	RTDepthDesc.Width = width;

	RTDepthDesc.Height = height;


	Application::GetApp()->GetDevice()->CreateTexture(RTDepthDesc, nullptr, &pRTDepth);
	// Store the depth-stencil view
	m_pDepthDSV = pRTDepth->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL);


	int a = 5;

}


void RenderTargetCube::Bind(int face) {

	BoundTarget = this;
	const float ClearColor[] = {mClearColor.x,mClearColor.y,mClearColor.z, 1.0f };
	Application* gApp = Application::GetApp();

	auto m_pImmediateContext = gApp->GetContext();


	RefCntAutoPtr<ITextureView> pRTV;
	

	pRTV = FaceView[face];



	m_pImmediateContext->SetRenderTargets(1, &pRTV, m_pDepthDSV /*RenderTargetCube::BoundTarget->GetDepthView()*/, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
	m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
	m_pImmediateContext->ClearDepthStencil(m_pDepthDSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);


}

void RenderTargetCube::Release(int face) {

	BoundTarget = nullptr;
	Application* gApp = Application::GetApp();

	auto m_pImmediateContext = gApp->GetContext();
	auto* pRTV = gApp->GetSwap()->GetCurrentBackBufferRTV();
	// Clear the default render target
	const float Zero[] = { 0.0f, 0.0f, 0, 1.0f };
	m_pImmediateContext->SetRenderTargets(1, &pRTV, gApp->GetSwap()->GetDepthBufferDSV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
//	m_pImmediateContext->ClearRenderTarget(pRTV, Zero, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);


}

RenderTargetCube* RenderTargetCube::BoundTarget = nullptr;