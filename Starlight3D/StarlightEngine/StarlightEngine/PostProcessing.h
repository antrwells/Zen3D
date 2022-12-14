#pragma once
#include "PostProcess.h"
#include <vector>

class SceneGraph;


class PostProcessing
{
public:

	PostProcessing(SceneGraph* graph);
	void SetPush(SceneGraph* mPushGraph);
	void AddPostProcess(PostProcess* pp);
	void PreRender();
	void Render();

private:

	std::vector<PostProcess*> mProcesses;
	SceneGraph* mGraph;
	SceneGraph* mPushGraph;

};

