#pragma once

#include <vector>
#include <string>
#include "assimp/scene.h"
#include "Common/interface/BasicMath.hpp"
#include "VFile.h"
using namespace Diligent;

struct KeyPosition
{
    float3 position;
    float timeStamp = 0.0f;
};

struct KeyRotation
{
    Diligent::Quaternion orientation;
    float timeStamp = 0.0f;
};

struct KeyScale
{
    float3 scale;
    float timeStamp = 0.0f;
};

float3 GetGLMVec(aiVector3D v);

Quaternion GetGLMQuat(aiQuaternion q);


/// <summary>
/// Used internally by NodeActor and other such classes to enable skeletal animations.
/// </summary>
class Bone
{
private:
    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
    std::vector<KeyScale> m_Scales;
    int m_NumPositions;
    int m_NumRotations;
    int m_NumScalings;

    float4x4 m_LocalTransform;
    std::string m_Name;
    int m_ID;

public:
    void Read(VFile* file) {

        m_NumPositions = file->ReadInt();
        for (int i = 0; i < m_NumPositions; i++)
        {
            KeyPosition kp;
            kp.position = file->ReadVec3();
            kp.timeStamp = file->ReadFloat();
            m_Positions.push_back(kp);
        }

        m_NumRotations = file->ReadInt();
        for (int i = 0; i < m_NumRotations; i++) {

            KeyRotation kr;

            float x = file->ReadFloat();
            float y = file->ReadFloat();
            float z = file->ReadFloat();
            float w = file->ReadFloat();

            Quaternion qt(x, y, z, w);

            kr.orientation = qt;
            kr.timeStamp = file->ReadFloat();

            m_Rotations.push_back(kr);

        }

        m_NumScalings = file->ReadInt();

        for (int i = 0; i < m_NumScalings; i++) {

            KeyScale ks;
            ks.scale = file->ReadVec3();
            ks.timeStamp = file->ReadFloat();
            m_Scales.push_back(ks);

        }

        m_LocalTransform = file->ReadMatrix();
        m_Name = std::string(file->ReadString());
        m_ID = file->ReadInt();

    }
    void Write(VFile* file) {

        file->WriteInt(m_NumPositions);
        for (int i = 0; i < m_NumPositions; i++) {

            auto pos = m_Positions[i];
            file->WriteVec3(pos.position);
            file->WriteFloat(pos.timeStamp);

        }

        file->WriteInt(m_NumRotations);
        for (int i = 0; i < m_NumRotations; i++) {

            auto rot = m_Rotations[i];
            //file->WriteVec3(pos.position);
            file->WriteFloat(rot.orientation.q.x);
            file->WriteFloat(rot.orientation.q.y);
            file->WriteFloat(rot.orientation.q.z);
            file->WriteFloat(rot.orientation.q.w);
            file->WriteFloat(rot.timeStamp);

        }

        file->WriteInt(m_NumScalings);
        for (int i = 0; i < m_NumScalings; i++) {

            auto scal = m_Scales[i];
            file->WriteVec3(scal.scale);
            file->WriteFloat(scal.timeStamp);

        }

        file->WriteMatrix(m_LocalTransform);
        file->WriteString(m_Name.c_str());
        file->WriteInt(m_ID);

    }

    Bone() {};
    /*reads keyframes from aiNodeAnim*/
    Bone(const std::string& name, int ID, const aiNodeAnim* channel)
        :
        m_Name(name),
        m_ID(ID),
        m_LocalTransform(1.0f)
    {
        m_NumPositions = channel->mNumPositionKeys;

        for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
        {
            aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
            float timeStamp = (float)channel->mPositionKeys[positionIndex].mTime;
            KeyPosition data;
            data.position = GetGLMVec(aiPosition);
           // data.position.x = -data.position.x;
            data.timeStamp = timeStamp;
            m_Positions.push_back(data);
        }

        m_NumRotations = channel->mNumRotationKeys;
        for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
        {
            aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
            float timeStamp = (float)channel->mRotationKeys[rotationIndex].mTime;
            KeyRotation data;
            data.orientation = GetGLMQuat(aiOrientation);
            data.timeStamp = timeStamp;
            m_Rotations.push_back(data);
        }

        m_NumScalings = channel->mNumScalingKeys;
        for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
        {
            aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
            float timeStamp = (float)channel->mScalingKeys[keyIndex].mTime;
            KeyScale data;
            data.scale = GetGLMVec(scale);
            data.timeStamp = timeStamp;
            m_Scales.push_back(data);
        }
    }

    /*interpolates  b/w positions,rotations & scaling keys based on the curren time of
    the animation and prepares the local transformation matrix by combining all keys
    tranformations*/
    void Update(float animationTime)
    {
        float4x4 translation = InterpolatePosition(animationTime);
        float4x4 rotation = InterpolateRotation(animationTime);
        float4x4 scale = InterpolateScaling(animationTime);
        //m_LocalTransform = translation * rotation;//*scale;
        m_LocalTransform = rotation * translation;      


    }



    float4x4 GetLocalTransform() { return m_LocalTransform; }
    std::string GetBoneName() const { return m_Name; }
    int GetBoneID() { return m_ID; }


    /* Gets the current index on mKeyPositions to interpolate to based on
    the current animation time*/
    int GetPositionIndex(float animationTime)
    {
        for (int index = 0; index < m_NumPositions - 1; ++index)
        {
            if (animationTime < m_Positions[index + 1].timeStamp)
                return index;
        }
        //assert(0);
    }

    /* Gets the current index on mKeyRotations to interpolate to based on the
    current animation time*/
    int GetRotationIndex(float animationTime)
    {
        for (int index = 0; index < m_NumRotations - 1; ++index)
        {
            if (animationTime < m_Rotations[index + 1].timeStamp)
                return index;
        }
    //    assert(0);
    }

    /* Gets the current index on mKeyScalings to interpolate to based on the
    current animation time */
    int GetScaleIndex(float animationTime)
    {
        for (int index = 0; index < m_NumScalings - 1; ++index)
        {
            if (animationTime < m_Scales[index + 1].timeStamp)
                return index;
        }
      //  assert(0);
    }

private:

    /* Gets normalized value for Lerp & Slerp*/
    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
    {
        float scaleFactor = 0.0f;
        float midWayLength = animationTime - lastTimeStamp;
        float framesDiff = nextTimeStamp - lastTimeStamp;
        scaleFactor = midWayLength / framesDiff;
        return scaleFactor;
    }

    /*figures out which position keys to interpolate b/w and performs the interpolation
    and returns the translation matrix*/
    float4x4 InterpolatePosition(float animationTime)
    {
        if (1 == m_NumPositions)
            return float4x4::Translation(m_Positions[0].position);

        int p0Index = GetPositionIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp,
            m_Positions[p1Index].timeStamp, animationTime);
        
        
        float nx = m_Positions[p0Index].position.x + (m_Positions[p1Index].position.x - m_Positions[p0Index].position.x) * scaleFactor;
        float ny = m_Positions[p0Index].position.y + (m_Positions[p1Index].position.y - m_Positions[p0Index].position.y) * scaleFactor;
        float nz = m_Positions[p0Index].position.z + (m_Positions[p1Index].position.z - m_Positions[p0Index].position.z) * scaleFactor;
        float3 finalPosition = Diligent::lerp(m_Positions[p0Index].position,
            m_Positions[p1Index].position, scaleFactor);
        return float4x4::Translation(finalPosition);
    }

    /*figures out which rotations keys to interpolate b/w and performs the interpolation
    and returns the rotation matrix*/
    float4x4 InterpolateRotation(float animationTime)
    {
        if (1 == m_NumRotations)
        {
            auto rotation = normalize(m_Rotations[0].orientation);
            return rotation.ToMatrix();   //glm::toMat4(rotation);
        }

        int p0Index = GetRotationIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp,
            m_Rotations[p1Index].timeStamp, animationTime);
        Quaternion finalRotation =Diligent::slerp(m_Rotations[p0Index].orientation,
            m_Rotations[p1Index].orientation, scaleFactor,true);
        finalRotation = normalize(finalRotation);
        return finalRotation.ToMatrix();

    }


    /*figures out which scaling keys to interpolate b/w and performs the interpolation
    and returns the scale matrix*/
    float4x4 InterpolateScaling(float animationTime)
    {

        if (1 == m_NumScalings)
            return float4x4::Scale(m_Scales[0].scale);

        int p0Index = GetScaleIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp,
            m_Scales[p1Index].timeStamp, animationTime);
        float3 finalScale = Diligent::lerp(m_Scales[p0Index].scale, m_Scales[p1Index].scale
            , scaleFactor);
        return  float4x4::Scale(finalScale);
    }

};
