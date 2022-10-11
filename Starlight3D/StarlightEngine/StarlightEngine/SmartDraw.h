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
#include "Texture2D.h"

//#include "Common/interface/BasicMath.hpp"


using namespace Diligent;

struct DrawInfo {

	float x[4];
	float y[4];
	float z;
	bool flip_uv = false;
	Texture2D* Tex;
	//Kinetic::Textures::Texture2D* Norm;
	float r, g, b, a;


};

/// <summary>
/// Internal use.
/// </summary>
struct DrawList {

	std::vector<DrawInfo*> Draws;
	Texture2D* Tex;
	//Kinetic::Textures::Texture2D* Norm;


};

struct Vertex2D
{
	float3 pos;
	float4 color;
	float2 uv;
};


class SmartDraw
{
public:

	SmartDraw(Application* app);
	void CreateVertexBuffer(DrawList* list);
	void CreateIndexBuffer(DrawList* list);

	void Begin();
	void DrawQuad(int x, int y, int w, int h, float r, float g, float b, float a);
	void DrawTexture(int x, int y, int w, int h, Texture2D* tex, float r, float g, float b, float a,bool flip_uv = false);
	void End(RefCntAutoPtr<IPipelineState> pso, RefCntAutoPtr<IBuffer> consts, RefCntAutoPtr<IShaderResourceBinding> srb);
	void End();
	void End(float4x4 proj);
	DrawList* GetDrawList(Texture2D* tex) {

		for (int i = 0; i < Draws.size(); i++) {

			DrawList* l = Draws[i];
			if (l->Tex == tex) {

				return l;

			}

		};

		DrawList* l = new DrawList;
		l->Tex = tex;
		//l->Norm = NULL;
		Draws.push_back(l);
		return l;

	};
	//DrawList* GetDrawList(Kinetic::Textures::Texture2D* tex);



private:
	Application* gApp;
	std::vector<DrawList*> Draws;
	float drawZ;
	RefCntAutoPtr<IPipelineState>         m_pPSO;
	RefCntAutoPtr<IBuffer>                m_VSConstants;
	RefCntAutoPtr<IShaderResourceBinding> m_pSRB;
	float4x4                              m_Proj;
	RefCntAutoPtr<IBuffer>                m_CubeVertexBuffer;
	RefCntAutoPtr<IBuffer>                m_CubeIndexBuffer;
	float cZ;
	bool made = false;
	Vertex2D* vertices;
	Uint32* indices;
};

