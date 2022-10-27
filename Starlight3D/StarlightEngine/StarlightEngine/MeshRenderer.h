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
#include "Graphics/GraphicsTools/interface/MapHelper.hpp"
#include "Common/interface/RefCntAutoPtr.hpp"
#include "Graphics/GraphicsTools/interface/GraphicsUtilities.h"

#include "Common/interface/BasicMath.hpp"

using namespace Diligent;
#include "NodeEntity.h"
#include "NodeLight.h"
#include "NodeCamera.h"
class NodeActor;
class MeshLines;

class MeshRenderer
{
public:

	MeshRenderer();
	void CreateActorGP();
	void CreateActorDepthGP();
	void CreateSimpleGP();
	void CreateLitGP();
	void CreateDepthGP();
	void CreateNormalsGP();
	void CreatePositionsGP();
	void CreateMeshLinesGP();
	void RenderMeshLines(MeshLines* mesh,NodeCamera* cam);
	void RenderActorDepth(NodeActor* actor, NodeCamera* cam);
	void RenderActor(NodeActor* actor, NodeCamera* cam, NodeLight* light, bool firstPass);
	void RenderPositions(NodeEntity* entity, NodeCamera* cam);
	void RenderNormals(NodeEntity* entity,NodeCamera* cam);
	void RenderSimple(NodeEntity* entity, NodeCamera* cam);
	void RenderLit(NodeEntity* entity, NodeCamera* cam, NodeLight* light,bool firstPass);
	void RenderDepth(NodeEntity* entity, NodeCamera* cam);
private:

	RefCntAutoPtr<IPipelineState>         m_PSO_Basic;
	RefCntAutoPtr<IPipelineState>		  m_PSO_Lit_FP;
	RefCntAutoPtr<IPipelineState>		  m_PSO_Lit_SP;
	RefCntAutoPtr<IPipelineState>		  m_PSO_Depth;
	RefCntAutoPtr<IPipelineState>		  m_PSO_Normals;
	RefCntAutoPtr<IPipelineState>		  m_PSO_Positions;
	RefCntAutoPtr<IPipelineState>		  m_PSO_Actor_FP;
	RefCntAutoPtr<IPipelineState>		  m_PSO_Actor_SP;
	RefCntAutoPtr<IPipelineState>		  m_PSO_Actor_Depth;
	RefCntAutoPtr<IPipelineState>		  m_PSO_MeshLines;
	RefCntAutoPtr<IBuffer> m_LitConstants;
	RefCntAutoPtr<IBuffer> m_NormalsConstants;
	RefCntAutoPtr<IBuffer> m_DepthConstants;
	RefCntAutoPtr<IBuffer> m_PositionsConstants;
	RefCntAutoPtr<IBuffer> m_ActorConstants;
	RefCntAutoPtr<IBuffer> m_ActorDepthConstants;
	RefCntAutoPtr<IBuffer> m_MeshLinesConstants;
	RefCntAutoPtr<IBuffer>                m_VSConstants;
	RefCntAutoPtr<IShaderResourceBinding> m_SRB_Basic;
	RefCntAutoPtr<IShaderResourceBinding> m_SRB_Lit;
	RefCntAutoPtr<IShaderResourceBinding> m_SRB_Depth;
	RefCntAutoPtr<IShaderResourceBinding> m_SRB_Normals;
	RefCntAutoPtr<IShaderResourceBinding> m_SRB_Positions;
	RefCntAutoPtr<IShaderResourceBinding> m_SRB_Actor;
	RefCntAutoPtr<IShaderResourceBinding> m_SRB_Actor_Depth;
	RefCntAutoPtr<IShaderResourceBinding> m_SRB_MeshLines;
	float4x4                              m_Final;



};

