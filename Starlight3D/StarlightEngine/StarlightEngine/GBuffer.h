#pragma once
#include "RenderTarget2D.h"

class SceneGraph;
class SmartDraw;

class GBuffer
{
public:

	GBuffer(int width, int height);
	void Render(SceneGraph* graph);
	void Debug(int id);

	RenderTarget2D* GetTextures();
	RenderTarget2D* GetNormals();
	RenderTarget2D* GetDepth();
	RenderTarget2D* GetPositions();

private:

	RenderTarget2D* mTexture;
	RenderTarget2D* mNormal;
	RenderTarget2D* mDepth;
	RenderTarget2D* mPositions;
	SmartDraw* mDraw;

};

