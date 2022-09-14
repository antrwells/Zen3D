#include "pch.h"
#include "NodeActor.h"
//#include "Animator.h"
#include "ActorAnim.h"
#include "NodeAnimator.h"



void NodeActor::SetAnimator(Animator* anim) {
	mAnimator = anim;	
}

	void NodeActor::UpdateAnim() {
		
		if (mCurrentAnim == nullptr) {
			return;
		}
		mCurrentAnim->Update();
		mAnimator->SetTime(mCurrentAnim->mCurTime);
	}

	void NodeActor::RenderDepth() {

		
	}

	void NodeActor::Render() {

	
		//RenderChildren();
	}


	void NodeActor::PlayAnim(std::string name) {

		mCurrentAnim = nullptr;

		for (int i = 0; i < mAnimations.size(); i++) {
			if (mAnimations[i]->mName == name) {
				mCurrentAnim = mAnimations[i];
				break;
			}
		}

		//if (mCurrentAnim != nullptr) {

		///}



	}
