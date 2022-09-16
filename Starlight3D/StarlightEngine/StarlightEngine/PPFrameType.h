#pragma once
#include "RenderTarget2D.h"

class Application;
class SceneGraph;

class PPFrameType
{
public:

	void SetGraph(SceneGraph* mGraph);

	virtual void CreateResources()
	{
		CreateFrame();
	}

	void CreateFrame(int width = -1, int height=-1);


	void StartFrame();
	void EndFrame();


	virtual void GenerateFrame() {};

	RenderTarget2D* GetTarget();
	

private:

	RenderTarget2D* mFrame;

protected:

	SceneGraph* mGraph;


};

