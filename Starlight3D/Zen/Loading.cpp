#include "ZenUI.h"
#include "Maths.h"
bool ZenUI::LoadPayload(FSPayload* pl, void(*done)(FSPayload* pl))
{

	if (mCurrentPayload != nullptr)
	{
		if (mCurrentPayload->LoadedPercent() >= 1.0f)
		{
			mWhenDone(mCurrentPayload);
			mCurrentPayload = pl;
			mCurrentPayload->LoadThreaded();
			mWhenDone = done;
			return true;
		}
		else {
			return false;
		}
	}

	mCurrentPayload = pl;
	mCurrentPayload->LoadThreaded();
	mWhenDone = done;
	return true;

}

float ZenUI::PayloadPercent() {

	if (mCurrentPayload == nullptr) {
		return 1.0f;
	}
	float lp = mCurrentPayload->LoadedPercent();
	if (lp >= 1.0f) {
		mWhenDone(mCurrentPayload);
		mCurrentPayload = nullptr;
	}
	return lp;

}

float cl = 1.0f;

void ZenUI::LoadingIndicator() {

	if (mCurrentPayload == nullptr) {
		return;

	}

	float com = mCurrentPayload->LoadedPercent();
	if (com >= 1.0f)
	{
		return;
	}

	ImGui::SetNextWindowPos(ImVec2(Application::GetApp()->GetWidth() - 90, Application::GetApp()->GetHeight() - 90));
	ImGui::SetNextWindowSize(ImVec2(70,70));




	bool open = true;
	ImGui::Begin("Progress",&open, ImGuiWindowFlags_NoDecoration);

	
	float cx, cy;

	cx = 32;
	cy = 32;

	int size = 25;

	cl = cl - 0.004f;
	if (cl < 0.f)
	{
		cl = 1.0f + cl;
	}

	for (int a = 0; a < 360; a += 20) {


	
		float dx, dy;
		dx = cx + cos(Maths::Deg2Rad((float)a-(cl*360))) * size;
		dy = cy + sin(Maths::Deg2Rad((float)a-(cl*360))) * size;

		float cv = 1.0f;

		cv = ((float)a) / 360.0f;
		cv = 1.0 - cv;

		cv = cv - cl;

		if (cv < 0.0f) {
			cv = 1.0 + cv;
		}
		//cv = 0.4;

		
		cv = 1.0 - cv;
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, cv, cv, cv));
		ImGui::SetCursorPos(ImVec2(dx, dy));
		ImGui::Image(mWhiteTex->GetView(), ImVec2(8, 8));
		ImGui::PopStyleColor();
		//draw->DrawTexture(dx - 5, dy - 5, 11, 11, white_tex, 0, cv, cv, cv, false);
		//printf("A:%d\n", a);
		// 
		//printf("CV:%f\n", cv);
	}


	//ImGui::ProgressBar(PayloadPercent());
	//ImGui::SameLine();

	//ImGui::Button("TESTING");


	ImGui::End();

}