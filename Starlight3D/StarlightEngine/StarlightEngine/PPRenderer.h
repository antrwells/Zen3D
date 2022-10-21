#pragma once

#ifndef PLATFORM_WIN32
#    define PLATFORM_WIN32 1
#endif

#include "Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h"
#include "Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h"
#include "Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h"
#include "Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"

#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Graphics/GraphicsEngine/interface/DeviceContext.h"
#include "Graphics/GraphicsEngine/interface/SwapChain.h"
#include "Graphics/GraphicsTools/interface/MapHelper.hpp"
#include "Common/interface/RefCntAutoPtr.hpp"
#include "Graphics/GraphicsTools/interface/GraphicsUtilities.h"
#include "Application.h"
//#include "Texture2D.h


//#include "Common/interface/BasicMath.hpp"


using namespace Diligent;

class SmartDraw;
class Texture2D;

class PPRenderer
{
public:

	PPRenderer();
	void CreateOutlineGP();
	void CreateColorLimitGP();
	void CreateBlurGP();
	void CreateCombineGP();
	void RenderOutline(Texture2D* texture, int w, int h, float diff, float inc, float4 color);
	void RenderCombine(Texture2D* texture,Texture2D* tex2, int w, int h, float i1, float i2);
	void RenderColorLimit(Texture2D* texture, int w, int h,float limit);
	void RenderBlur(Texture2D* texture, int w, int h, float blur);

private:
	RefCntAutoPtr<IPipelineState>         m_pPSO_ColorLimit;
	RefCntAutoPtr<IBuffer>                m_VSConstants_ColorLimit;
	RefCntAutoPtr<IShaderResourceBinding> m_pSRB_ColorLimit;
	RefCntAutoPtr<IPipelineState>         m_pPSO_Blur;
	RefCntAutoPtr<IBuffer>                m_VSConstants_Blur;
	RefCntAutoPtr<IShaderResourceBinding> m_pSRB_Blur;
	RefCntAutoPtr<IPipelineState>         m_pPSO_Combine;
	RefCntAutoPtr<IBuffer>                m_VSConstants_Combine;
	RefCntAutoPtr<IShaderResourceBinding> m_pSRB_Combine;
	RefCntAutoPtr<IPipelineState> m_pPSO_Outline;
	RefCntAutoPtr<IBuffer> m_VSConstants_Outline;
	RefCntAutoPtr<IShaderResourceBinding> m_pSRB_Outline;

	SmartDraw* mDraw;

};

