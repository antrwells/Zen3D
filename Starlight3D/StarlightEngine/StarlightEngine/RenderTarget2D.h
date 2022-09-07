#pragma once
#ifndef PLATFORM_WIN32
#    define PLATFORM_WIN32 1
#endif

#ifndef ENGINE_DLL
#    define ENGINE_DLL 1
#endif

#ifndef D3D11_SUPPORTED
#    define D3D11_SUPPORTED 1
#endif

#ifndef D3D12_SUPPORTED
#    define D3D12_SUPPORTED 1
#endif

#ifndef GL_SUPPORTED
#    define GL_SUPPORTED 1
#endif

#ifndef VULKAN_SUPPORTED
#    define VULKAN_SUPPORTED 1
#endif


#include "Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h"
#include "Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h"
#include "Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h"
#include "Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"

#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Graphics/GraphicsEngine/interface/DeviceContext.h"
#include "Graphics/GraphicsEngine/interface/SwapChain.h"

#include "Common/interface/RefCntAutoPtr.hpp"

#include "Common/interface/BasicMath.hpp"

using namespace Diligent;

class RenderTarget2D
{
public:

	RenderTarget2D(int width, int height);
	void Bind();
	void Release();
	RefCntAutoPtr<ITextureView> GetColorView() { return m_pColorRTV; }
	RefCntAutoPtr<ITextureView> GetDepthView() { return m_pDepthDSV; }
	RefCntAutoPtr<ITexture> GetColorTexture() { return pRTColor; }

	static RenderTarget2D* BoundTarget;

private:

	RefCntAutoPtr<ITexture> pRTColor;
	RefCntAutoPtr<ITexture> pRTDepth;
	RefCntAutoPtr<ITextureView> m_pColorRTV;
	RefCntAutoPtr<ITextureView> m_pDepthDSV;

};

