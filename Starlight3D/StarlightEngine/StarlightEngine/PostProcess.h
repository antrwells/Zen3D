#pragma once
#include <vector>
#include "PPFrameType.h"

class SceneGraph;
class SmartDraw;
class PPRenderer;

class PostProcess
{
public:

	PostProcess();
	void AddFrameType(PPFrameType* frame);
	void GenerateFrames();
	PPFrameType* GetFrame(int index);
	void SetGraph(SceneGraph* graph);
	void Draw(RenderTarget2D* texture, int width=-1, int height=-1);
	virtual void PP() {};

	static PPRenderer* GetRenderer();
	void InitTargets(int width, int height, int number);
	void BindTarget(int index);
	void ReleaseTarget(int index);
	RenderTarget2D* GetTarget(int index) {
		return mTargets[index];
	}
private:
	
	std::vector<PPFrameType*> mFrames;
	static PPRenderer* mRenderer;


protected:
	
	std::vector<RenderTarget2D*> mTargets;
	SceneGraph* mGraph;
	SmartDraw* mDraw;

};

