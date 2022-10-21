#include "pch.h"
#include "PostProcessing.h"

PostProcessing::PostProcessing(SceneGraph* graph) {

	mGraph = graph;

}

void PostProcessing::AddPostProcess(PostProcess* pp)
{

	mProcesses.push_back(pp);
	pp->SetGraph(mGraph);

}

void PostProcessing::SetPush(SceneGraph* graph) {

	mPushGraph = graph;

}

void PostProcessing::PreRender() {
	for (int i = 0; i < mProcesses.size(); i++) {

		auto pp = mProcesses[i];
		pp->GenerateFrames();
		pp->PP();


		//pp->Draw(pp->GetFrame(0)->GetTarget());


	}

}

void PostProcessing::Render() {

	for (int i = 0; i < mProcesses.size(); i++)
	{

		auto pp = mProcesses[i];

		pp->Render();

	}

}