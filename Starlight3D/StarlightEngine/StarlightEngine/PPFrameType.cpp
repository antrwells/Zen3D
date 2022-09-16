#include "pch.h"
#include "PPFrameType.h"
#include "Application.h"

void PPFrameType::SetGraph(SceneGraph* graph) {

	mGraph = graph;

}

void PPFrameType::CreateFrame(int width,int height) {

	if (width == -1)
	{
		width = Application::GetApp()->GetWidth();
		height = Application::GetApp()->GetHeight();

	}

	mFrame = new RenderTarget2D(width, height);

}

void PPFrameType::StartFrame() {

	mFrame->Bind();

}

void PPFrameType::EndFrame() {

	mFrame->Release();

}

RenderTarget2D* PPFrameType::GetTarget() {

	return mFrame;

}