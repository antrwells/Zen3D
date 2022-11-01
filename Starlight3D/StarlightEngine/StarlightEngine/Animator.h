#pragma once
#include "Animation.h"
#include "VFile.h"


class Animator
{
public:
    Animator() {};
    Animator(Animation* Animation)
    {
        m_CurrentTime = 0.0;
        m_CurrentAnimation = Animation;

        m_FinalBoneMatrices.reserve(100);

        for (int i = 0; i < 100; i++)
            m_FinalBoneMatrices.push_back(float4x4::Identity());
    }

    void SetTime(float t) {

        m_CurrentTime = t;
        CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), float4x4::Identity());

    }

    void UpdateAnimation(float dt)
    {
        m_DeltaTime = dt;
        if (m_CurrentAnimation)
        {
            m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
            m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
            if (m_CurrentTime >= (m_CurrentAnimation->GetDuration()-1.0f)) {

            }
            else {
                CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), float4x4::Identity());
            }
        }
    }

    void PlayAnimation(Animation* pAnimation)
    {
        m_CurrentAnimation = pAnimation;
        m_CurrentTime = 0.0f;
    }

    void CalculateBoneTransform(const AssimpNodeData* node,float4x4 parentTransform)
    {
        std::string nodeName = node->name;
        float4x4 nodeTransform = node->transformation;

        Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

        if (Bone)
        {
            Bone->Update(m_CurrentTime);
            nodeTransform = Bone->GetLocalTransform();
        }
        //<<<
        float4x4 globalTransformation = nodeTransform * parentTransform;
        //float4x4 globalTransformation = parentTransform * nodeTransform;


        auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        {
            int index = boneInfoMap[nodeName].id;
            float4x4 offset = boneInfoMap[nodeName].offset;
            m_FinalBoneMatrices[index] = offset * globalTransformation;
            //m_FinalBoneMatrices[index] = globalTransformation


        }

        for (int i = 0; i < node->childrenCount; i++)
            CalculateBoneTransform(&node->children[i], globalTransformation);
    }

    std::vector<float4x4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

    Animation* GetAnimation() {
        return m_CurrentAnimation;
    }

    void ResetBones() {
        for(int i=0;i<m_FinalBoneMatrices.size();i++){
            m_FinalBoneMatrices[i] = float4x4::Identity();
        }
    }
    void Write(VFile* file) {

        file->WriteInt((int)m_FinalBoneMatrices.size());
        for (int i = 0; i < m_FinalBoneMatrices.size(); i++) {
            file->WriteMatrix(m_FinalBoneMatrices[i]);
        }
        m_CurrentAnimation->Write(file);
        file->WriteFloat(m_CurrentTime);
        file->WriteFloat(m_DeltaTime);
    }
    void Read(VFile* file) {

        int size = file->ReadInt();
        for (int i = 0; i < size; i++) {
            float4x4 mat = file->ReadMatrix();
            m_FinalBoneMatrices.push_back(mat);
        }
        m_CurrentAnimation = new Animation;
        m_CurrentAnimation->Read(file);
        m_CurrentTime = file->ReadFloat();
        m_DeltaTime = file->ReadFloat();
    }
private:
    std::vector<float4x4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation = nullptr;
    float m_CurrentTime = 0.0f;
    float m_DeltaTime = 0.0f;
};
