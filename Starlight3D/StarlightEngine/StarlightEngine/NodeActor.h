#pragma once
#include "NodeEntity.h"
//#include <map>

#include "BoneInfo.h"
#include <string>
#include "ActorAnim.h"
#include <vector>
#include <thread>
#include <mutex>
class Animator;


    class NodeActor;

    void Thr_Anim(NodeActor* actor);

    /// <summary>
    /// NodeActors are essentially like NodeEntity's, except they have skeletal animation support for your in game/app characters.
    /// You can import these using Importer::ImportActor(...)...
    /// These animations are performed on the gpu.
    /// </summary>
    class NodeActor :
        public NodeEntity
    {
    public:

       

        /// <summary>
        /// Internal use only.
        /// </summary>
        /// <returns></returns>
      //  auto& GetBoneInfoMap() { return m_BoneInfoMap; }
        /// <summary>
        /// Internal Use Only.
        /// </summary>
        /// <returns></returns>
        int& GetBoneCount() { return m_BoneCounter; }
        /// <summary>
        /// Internal use only.
        /// </summary>
        /// <param name="map"></param>
        /// <param name="count"></param>
    //    void SetBoneInfoMap(std::map<std::string, BoneInfo> map, int count) {

          //  m_BoneInfoMap = map;
         //   m_BoneCounter = count;
//
  //      }

        /// <summary>
        /// Internal use only.
        /// </summary>
        /// <param name="anim"></param>
        void SetAnimator(Animator* anim) {
            mAnimator = anim;
        }

        /// <summary>
        /// Updates the animation time by "t" amount.
        /// </summary>
        /// <param name="t"></param>
        void UpdateAnim();


        /// <summary>
         /// Internal use.
         /// </summary>
        void RenderDepth();

        /// <summary>
          /// Internal use.
          /// </summary>
        void Render();

        /// <summary>
        /// adds an animation data block to the actor. such as start/end frames and speed of animation.
        /// </summary>
        /// <param name="anim"></param>
        void AddAnim(ActorAnim* anim) {
            mAnimations.push_back(anim);
        }


        /// <summary>
        /// Begins playing the animation.
        /// </summary>
        /// <param name="name"></param>
        void PlayAnim(std::string name) {

            mCurrentAnim = nullptr;

            for (int i = 0; i < mAnimations.size(); i++) {
                if (mAnimations[i]->mName == name) {
                    mCurrentAnim = mAnimations[i];
                    break;
                }
            }

            if (mCurrentAnim != nullptr) {

            }

           

        }

     
    private:
       // std::map<std::string,BoneInfo> m_BoneInfoMap; //
        int m_BoneCounter = 0;
        Animator* mAnimator;
        std::vector<ActorAnim*> mAnimations;
        ActorAnim* mCurrentAnim;

       // NodeAnimator* mAnimator;
    };


