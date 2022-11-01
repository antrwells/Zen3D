#pragma once
#include "pch.h"
#include "NodeActor.h"
//#include "Animator.h"
#include "ActorAnim.h"
#include "NodeAnimator.h"
#include "Animator.h"


void NodeActor::ReadAnimations(VFile* file) {

	m_BoneCounter = file->ReadInt();
	int size = file->ReadInt();
	for (int i = 0; i < size; i++) {

		std::string name = std::string(file->ReadString());
		BoneInfo info;
		info.Read(file);
		m_BoneInfoMap.insert(std::make_pair(name, info));

	}
	mAnimator = new Animator;
	mAnimator->Read(file);
	int ac = file->ReadInt();
	for (int i = 0; i < ac; i++) {
		ActorAnim* n_anim = new ActorAnim("", 0, 0, 0, (AnimType)0);
		n_anim->Read(file);
		mAnimations.push_back(n_anim);
	}
}

void NodeActor::WriteAnimations(VFile* file) {

	file->WriteInt(m_BoneCounter);
	std::map<std::string, BoneInfo>::iterator itr;
	file->WriteInt((int)m_BoneInfoMap.size());
	for (itr = m_BoneInfoMap.begin(); itr != m_BoneInfoMap.end(); ++itr) {
		// cout << '\t' << itr->first << '\t' << itr->second
		 //    << '\n';

		file->WriteString(itr->first.c_str());
		itr->second.Write(file);
	}
	mAnimator->Write(file);
	file->WriteInt((int)mAnimations.size());
	for (int i = 0; i < mAnimations.size(); i++) {
		mAnimations[i]->Write(file);
	}


}

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

	void NodeActor::StopAnim() {
		if (mCurrentAnim == nullptr) return;
		mCurrentAnim->mCurTime = mCurrentAnim->mStartTime;
		
		mAnimator->UpdateAnimation(mCurrentAnim->mStartTime);
		mAnimator->SetTime(mCurrentAnim->mStartTime);
		mAnimator->ResetBones();
		mCurrentAnim = nullptr;


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
