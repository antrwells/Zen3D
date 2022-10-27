#pragma once
#include "Common/interface/BasicMath.hpp"
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
using namespace Diligent;
struct LinesVertex {

	float3 position;
	float4 color;

};

struct Line {

	int v0, v1;

};

class MeshLines
{
public:

	MeshLines();
	void AddLine(float3 p1, float3 p2,float4 col) {

		LinesVertex v0, v1;
		v0.position = p1;
		v0.color = col;
		v1.position = p2;
		v1.color = col;
		mVertices.push_back(v0);
		mVertices.push_back(v1);
		Line l;
		l.v0 = mVertices.size() - 2;
		l.v1 = mVertices.size() - 1;
		mLines.push_back(l);

	}

	std::vector<Line> GetLines() {
		return mLines;
	}

	RefCntAutoPtr<IBuffer> GetVertexBuffer() {

		return m_VertexBuffer;

	}

	RefCntAutoPtr<IBuffer> GetIndexBuffer() {

		return m_IndexBuffer;

	}

	void CreateBuffers();

private:
	RefCntAutoPtr<IBuffer>                m_VertexBuffer;
	RefCntAutoPtr<IBufferView> m_VertexBufferView;
	RefCntAutoPtr<IBuffer>                m_IndexBuffer;
	RefCntAutoPtr<IBufferView> m_IndexBufferView;
	std::vector<LinesVertex> mVertices;
	std::vector<Line> mLines;


};

