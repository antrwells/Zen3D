#include "pch.h"
#include "PostProcess.h"
#include "SmartDraw.h"
#include "PPRenderer.h"

PostProcess::PostProcess() {

	mDraw = new SmartDraw(Application::GetApp());

	if (mRenderer == nullptr) {

		mRenderer = new PPRenderer;

	}

}

void PostProcess::AddFrameType(PPFrameType* frame) {

	mFrames.push_back(frame);

}

void PostProcess::GenerateFrames() {

	for (int i = 0;i < mFrames.size();i++) {

		auto frame = mFrames[i];

		frame->StartFrame();

		frame->GenerateFrame();

		frame->EndFrame();

	}

}

PPFrameType* PostProcess::GetFrame(int index) {

	return mFrames[index];

}

void PostProcess::SetGraph(SceneGraph* graph) {

	mGraph = graph;
	for (int i = 0;i < mFrames.size();i++) {

		mFrames[i]->SetGraph(graph);

	}


}


void PostProcess::Draw(RenderTarget2D* target, int width, int height) {

	if (width == -1) {

		width = Application::GetApp()->GetWidth();
		height = Application::GetApp()->GetHeight();

	}

	mDraw->Begin();

	mDraw->DrawTexture(0, 0, width, height, target->ToTexture2D(), 1, 1, 1, 1, false);

	mDraw->End();


}

PPRenderer* PostProcess::GetRenderer() {

	return mRenderer;

}

void PostProcess::InitTargets(int width, int height, int number) {

	for (int i = 0;i < number;i++) {

		mTargets.push_back(new RenderTarget2D(width, height));

	}

}

void PostProcess::BindTarget(int index) {

	mTargets[index]->Bind();

}

void PostProcess::ReleaseTarget(int index) {

	mTargets[index]->Release();

}

PPRenderer* PostProcess::mRenderer = nullptr;