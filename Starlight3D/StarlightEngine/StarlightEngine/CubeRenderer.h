#pragma once
#include "RenderTargetCube.h"
#include "SceneGraph.h"
class CubeRenderer
{
public:

	CubeRenderer(SceneGraph* graph, RenderTargetCube* cube);
	void Render(float3 position);
	void RenderDepth(float3 position,float maxZ);
	void SetRenderTargetCube(RenderTargetCube* cube);
	TextureCube* GetTextureCube();

private:

	TextureCube* mTexCube = nullptr;
	SceneGraph* mGraph;
	RenderTargetCube* mRT;

};

