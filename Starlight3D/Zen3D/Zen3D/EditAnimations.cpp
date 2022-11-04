#include "ZenUI.h"
#include "NodeActor.h"
#include "ActorAnim.h"
#include "Animator.h"
#include "Animation.h"

void ZenUI::EditAnimationsWindow() {


	if (mEditAnimsFirst) {

		ImGui::SetNextWindowPos(ImVec2(120, 120));
		ImGui::SetNextWindowSize(ImVec2(400, 500));
		mEditAnimsFirst = false;

	}

	std::string title = "Edit Animations:" + std::string(mEditAnimation->GetName());

	if (ImGui::Begin(title.c_str(), &mEditAnimsOpen)) {

		
		auto anims = mEditAnimation->GetAnims();
		ImGui::BeginChild(128);
		for (int i = 0; i < anims.size(); i++)
		{
			std::string animTitle = "Animation:" + std::string(anims[i]->mName);
	
			ImGui::PushID(i);
			
			ImGui::Text(animTitle.c_str());

			ImGui::Spacing();

			auto anim = anims[i];

			char* buf = (char*)malloc(2048);
			auto name = anim->mName.c_str();

			for (int i = 0; i < 2048; i++) {

				int ch = name[i];
				if (ch == "\0"[0])
				{
					buf[i] = "\0"[0];
					break;
				}
				buf[i] = ch;

			}


			if (ImGui::InputText("Name", buf, 2048))
			{

				anim->mName = std::string(VString(buf).GetConst());

			}

			auto anima = mEditAnimation->GetAnimator();
			auto dura = anima->GetAnimation();
			int max = (int)dura->GetDuration();

			int f1, f2;

			f1 = anim->mStartTime;
			f2 = anim->mEndTime;

			if (ImGui::DragInt("Frame Start", &f1, 1, 0, max))
			{
				anim->mStartTime = f1;
			}

			if (ImGui::DragInt("Frame End", &f2, 1, 0, max)) {
				anim->mEndTime = f2;
			}

			float spd = anim->mSpeed;

			if (ImGui::DragFloat("Speed", &spd, 0.01f, 0.001f, 10.0f))
			{
				anim->mSpeed = spd;
			}

			if (ImGui::Button("Play Animation"))
			{

				mEditAnimation->PlayAnim(anim->mName);

			}

			ImGui::PopID();
		}

		mEditAnimation->UpdateAnim();

		if (ImGui::Button("New Animation"))
		{
			ActorAnim* new_anim = new ActorAnim("", 0, 1, 0.1f, AnimType::Forward);
			mEditAnimation->AddAnim(new_anim);
			//anims.push_back(new_anim);
			
		}

		ImGui::EndChild();

		ImGui::End();
	}



}