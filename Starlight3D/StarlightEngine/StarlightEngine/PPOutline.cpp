#include "pch.h"
#include "PPOutline.h"
#include "FrameTypeColor.h"
#include "PPRenderer.h"
#include "Application.h"
#include "SmartDraw.h"
#include "RenderTarget2D.h"
#include "SceneGraph.h"
PPOutline::PPOutline() {

	//AddFrameType(new FrameTypeColor())
//	auto frame_color = new FrameTypeColor();
	//AddFrameType(frame_color)
	//auto frame_depth = new Fra	;
	InitTargets(Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight(), 2);
	//frame_color->SetGraph(graph);

	//mGraph = graph;


}



void PPOutline::PP() {

	auto p_ren = PostProcess::GetRenderer();

	BindTarget(0);

		mGraph->RenderDepth();

	ReleaseTarget(0);

	BindTarget(1);

	p_ren->RenderOutline(GetTarget(0)->ToTexture2D(), Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight(),0.02f,0.001f,float4(0.2f,1,0.2f,1));

	ReleaseTarget(1);


}

void PPOutline::Render() {
	auto p_ren = PostProcess::GetRenderer();
	
	mDraw->Begin();
	mDraw->DrawTexture(0, 0, Application::GetApp()->GetWidth(),Application::GetApp()->GetHeight(), new Texture2D(mTargets[1]), 1, 1, 1, 1, false);
	mDraw->End();

	//p_ren->RenderCombine(GetTarget(1)->ToTexture2D(), GetFrame(0)->GetTarget()->ToTexture2D(), Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight(), 1, 1);


}