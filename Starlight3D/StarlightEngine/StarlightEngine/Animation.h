#pragma once

//#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <vector>
#include <string>
#include "NodeActor.h"
#include "BoneInfo.h"
#include "Bone.h"

inline float4x4 aiMatrix4x4ToGlm(const aiMatrix4x4* from)
{
    float4x4 to;

    ///***
    /*
    to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
    to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
    to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
    to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;
    */
    return to;
}

struct AssimpNodeData
{
    float4x4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};


/// <summary>
/// The animation class holds all the animation keys for any given NodeActor.
/// This class is used internally. You should generally have no need to use it directly yourself, unless expanding the engine.
/// </summary>
class Animation
{
public:
    Animation() = default;

    Animation(aiScene* scene,NodeActor* model)
    {

     //   const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
        //assert(scene && scene->mRootNode);
        auto animation = scene->mAnimations[0];
        m_Duration = animation->mDuration;
        m_TicksPerSecond = animation->mTicksPerSecond;
        ReadHeirarchyData(m_RootNode, scene->mRootNode);
    //    ReadMissingBones(animation, model);
    }

    ~Animation()
    {
    }

    Bone* FindBone(const std::string& name)
    {
        auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
            [&](const Bone& Bone)
            {
                return Bone.GetBoneName() == name;
            }
        );
        if (iter == m_Bones.end()) return nullptr;
        else return &(*iter);
    }


    inline float GetTicksPerSecond() { return m_TicksPerSecond; }

    inline float GetDuration() { return m_Duration; }

    inline const AssimpNodeData& GetRootNode() { return m_RootNode; }

  //  inline const std::map<std::string, BoneInfo>& GetBoneIDMap()
   // {
    //    return m_BoneInfoMap;
    //}

private:
    void ReadMissingBones(const aiAnimation* animation, NodeActor* model)
    {
        //***
     /*
        int size = animation->mNumChannels;

        auto& boneInfoMap = model->GetBoneInfoMap();//getting m_BoneInfoMap from Model class
        int& boneCount = model->GetBoneCount(); //getting the m_BoneCounter from Model class

        //reading channels(bones engaged in an animation and their keyframes)
        for (int i = 0; i < size; i++)
        {
            auto channel = animation->mChannels[i];
            std::string boneName = channel->mNodeName.data;

            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                boneInfoMap[boneName].id = boneCount;
                boneCount++;
            }
            m_Bones.push_back(Bone(channel->mNodeName.data,
                boneInfoMap[channel->mNodeName.data].id, channel));
        }

        m_BoneInfoMap = boneInfoMap;
        */
    }

    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
    {
       // assert(src);

        dest.name = src->mName.data;
        dest.transformation = aiMatrix4x4ToGlm(&src->mTransformation);
        dest.childrenCount = src->mNumChildren;

        for (int i = 0; i < src->mNumChildren; i++)
        {
            AssimpNodeData newData;
            ReadHeirarchyData(newData, src->mChildren[i]);
            dest.children.push_back(newData);
        }
    }
    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    //std::map<std::string, BoneInfo> m_BoneInfoMap;
};