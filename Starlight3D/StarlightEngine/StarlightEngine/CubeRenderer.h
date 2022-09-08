#pragma once
#include "RenderTargetCube.h"
#include "SceneGraph.h"
class CubeRenderer
{
public:

	CubeRenderer(SceneGraph* graph, RenderTargetCube* cube);
	void Render(float3 position);
	TextureCube* GetTextureCube();

private:

	TextureCube* mTexCube = nullptr;
	SceneGraph* mGraph;
	RenderTargetCube* mRT;

};

