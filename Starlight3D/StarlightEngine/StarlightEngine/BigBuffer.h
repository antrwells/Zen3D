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
#include "DataTypes.h"

using namespace Diligent;



class BigBuffer
{
public:

	BigBuffer() {};
	void AddData(std::vector<Vertex> vertices, std::vector<Tri> tris);
	void CreateBuffers();

	RefCntAutoPtr<IBufferView> GetVertexBufferView();
	RefCntAutoPtr<IBufferView> GetIndexBufferView();	
	RefCntAutoPtr<IBufferView> GetGeoIndexBufferView();


private:
	std::vector<Vertex> mVertices;
	std::vector<Tri> mTris;
	std::vector<GeoIndex> mGeos;

	RefCntAutoPtr<IBuffer> mVertexBuffer;
	RefCntAutoPtr<IBufferView> mVertexBufferView;
	RefCntAutoPtr<IBuffer> mGeoBuffer;
	RefCntAutoPtr<IBufferView> mGeoBufferView;
	RefCntAutoPtr<IBuffer> mIndexBuffer;
	RefCntAutoPtr<IBufferView> mIndexBufferView;
};


