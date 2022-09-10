#include "pch.h"
#include "RenderTarget2D.h"
#include "Application.h"

RenderTarget2D::RenderTarget2D(int width, int height) {


	//color
	TextureDesc RTColorDesc;
	RTColorDesc.Name = "RenderTarget Texture";
	RTColorDesc.Type = RESOURCE_DIM_TEX_2D;
	RTColorDesc.Width = width;// pSwapChain->GetDesc().Width;
	RTColorDesc.Height = height;// pSwapChain->GetDesc().Height;
	RTColorDesc.MipLevels = 1;
	RTColorDesc.Format = TEX_FORMAT_RGBA16_FLOAT;
	RTColorDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
	RTColorDesc.ClearValue.Format = RTColorDesc.Format;
	RTColorDesc.ClearValue.Color[0] = 0.650f;
	RTColorDesc.ClearValue.Color[1] = 0.150f;
	RTColorDesc.ClearValue.Color[2] = 0.150f;
	RTColorDesc.ClearValue.Color[3] = 1.f;
	Application::GetApp()->GetDevice()->CreateTexture(RTColorDesc, nullptr, &pRTColor);
	m_pColorRTV = pRTColor->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET);

	//depth
	TextureDesc RTDepthDesc = RTColorDesc;
	RTDepthDesc.Name = "Cube depth buffer";
	RTDepthDesc.Format = TEX_FORMAT_D32_FLOAT;
	RTDepthDesc.BindFlags = BIND_DEPTH_STENCIL;
	// Define optimal clear value
	RTDepthDesc.ClearValue.Format = RTDepthDesc.Format;
	RTDepthDesc.ClearValue.DepthStencil.Depth = 1;
	RTDepthDesc.ClearValue.DepthStencil.Stencil = 0;

	Application::GetApp()->GetDevice()->CreateTexture(RTDepthDesc, nullptr, &pRTDepth);
	// Store the depth-stencil view
	m_pDepthDSV = pRTDepth->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL);
	mClearColor = float4(0,0,0,1);


}


void RenderTarget2D::Bind() {

	BoundTarget = this;
	const float ClearColor[] = { mClearColor.x,mClearColor.y,mClearColor.z, 1.0f };
	Application* gApp = Application::GetApp();

	auto m_pImmediateContext = gApp->GetContext();
	m_pImmediateContext->SetRenderTargets(1, &RenderTarget2D::BoundTarget->GetColorView(), RenderTarget2D::BoundTarget->GetDepthView(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
	m_pImmediateContext->ClearRenderTarget(RenderTarget2D::BoundTarget->GetColorView(), ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
	m_pImmediateContext->ClearDepthStencil(RenderTarget2D::BoundTarget->GetDepthView(), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

}

void RenderTarget2D::Release() {

	BoundTarget = nullptr;
	Application* gApp = Application::GetApp();

	auto m_pImmediateContext = gApp->GetContext();
	auto* pRTV = gApp->GetSwap()->GetCurrentBackBufferRTV();
	// Clear the default render target
	const float Zero[] = { 0, 0, 0, 1.0f };
	m_pImmediateContext->SetRenderTargets(1, &pRTV, gApp->GetSwap()->GetDepthBufferDSV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
	//m_pImmediateContext->ClearRenderTarget(pRTV, Zero, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);


}

RenderTarget2D* RenderTarget2D::BoundTarget = nullptr;