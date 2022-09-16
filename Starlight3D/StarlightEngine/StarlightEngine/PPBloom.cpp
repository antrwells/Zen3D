#include "pch.h"
#include "PPBloom.h"
#include "FrameTypeColor.h"
#include "PPRenderer.h"
#include "Application.h"
PPBloom::PPBloom() {

	//AddFrameType(new FrameTypeColor())
	auto frame_color = new FrameTypeColor();
	AddFrameType(frame_color);
	InitTargets(Application::GetApp()->GetWidth(),Application::GetApp()->GetHeight(), 2);
	//frame_color->SetGraph(graph);

	//mGraph = graph;


}

void PPBloom::PP() {

	auto p_ren = PostProcess::GetRenderer();

	BindTarget(0);

	p_ren->RenderColorLimit(GetFrame(0)->GetTarget()->ToTexture2D(), Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight(), 0.3);

	ReleaseTarget(0);

	BindTarget(1);

	p_ren->RenderBlur(GetTarget(0)->ToTexture2D(), Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight(), 0.0035f);

	ReleaseTarget(1);

	p_ren->RenderCombine(GetTarget(1)->ToTexture2D(), GetFrame(0)->GetTarget()->ToTexture2D(), Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight(), 1.5f, 1.0f);
	


	//Draw(GetTarget(1));





	int a = 5;

}