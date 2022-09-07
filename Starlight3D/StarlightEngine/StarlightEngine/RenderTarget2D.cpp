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
	RTColorDesc.Format = TEX_FORMAT_RGBA8_UNORM;
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
	RTDepthDesc.Name = "Offscreen depth buffer";
	RTDepthDesc.Format = TEX_FORMAT_D32_FLOAT;
	RTDepthDesc.BindFlags = BIND_DEPTH_STENCIL;
	// Define optimal clear value
	RTDepthDesc.ClearValue.Format = RTDepthDesc.Format;
	RTDepthDesc.ClearValue.DepthStencil.Depth = 1;
	RTDepthDesc.ClearValue.DepthStencil.Stencil = 0;

	Application::GetApp()->GetDevice()->CreateTexture(RTDepthDesc, nullptr, &pRTDepth);
	// Store the depth-stencil view
	m_pDepthDSV = pRTDepth->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL);

}


void RenderTarget2D::Bind() {

	BoundTarget = this;

}

void RenderTarget2D::Release() {

	BoundTarget = nullptr;

}

RenderTarget2D* RenderTarget2D::BoundTarget = nullptr;