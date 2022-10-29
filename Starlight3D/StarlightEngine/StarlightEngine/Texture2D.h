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
#include "TextureLoader/interface/TextureUtilities.h"
#include "Application.h"

#include "Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h"
#include "Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h"
#include "Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h"
#include "Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"

#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Graphics/GraphicsEngine/interface/DeviceContext.h"
#include "Graphics/GraphicsEngine/interface/SwapChain.h"
#include "Graphics/GraphicsEngine/interface/Texture.h"

#include "Common/interface/RefCntAutoPtr.hpp"

#include "Common/interface/BasicMath.hpp"
#if PLATFORM_WIN32
#    define GLFW_EXPOSE_NATIVE_WIN32 1
#endif
//#include "GLFW/glfw3.h"
//#include "GLFW/glfw3native.h"
#include "Application.h"
#include "RenderTarget2D.h"
#include "RenderTargetCube.h"
#include <vector>
using namespace Diligent;


//#include "Common/interface/BasicMath.hpp"



class Texture2D
{
public:
	Texture2D(const char* path,bool alpha = false);
	Texture2D(RefCntAutoPtr<ITexture> texture, RefCntAutoPtr<ITextureView> view);
	Texture2D(RefCntAutoPtr<ITexture> texure);
	Texture2D(Application* app,int w, int h, bool alpha,const char* buf);
	Texture2D(RenderTarget2D* target);
	Texture2D(RenderTargetCube* cube, int face);
	int GetWidth() {
		return mWidth;
	}
	int GetHeight() {
		return mHeight;
	}
	RefCntAutoPtr<ITextureView> GetView() {
		return m_TextureSRV;
	}
	RefCntAutoPtr<ITexture> GetTexture() {
		return Texture;
	}
	IDeviceObject* GetViewPTR() {

		return Texture->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

	}
	std::string GetPath() {
		return mPath;
	}
private:

	RefCntAutoPtr<ITexture> Texture;
	RefCntAutoPtr<ITextureView>           m_TextureSRV;
	int mWidth;
	int mHeight;
	std::string mPath;
	static std::vector<Texture2D*> mCache;
};


