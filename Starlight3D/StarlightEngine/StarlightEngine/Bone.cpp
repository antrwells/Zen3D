#include "pch.h"
#include "Bone.h"
float3 GetGLMVec(aiVector3D v) {

    return float3(v.x, v.y, v.z);

}

Quaternion GetGLMQuat(aiQuaternion q) {

    return Quaternion(q.x, q.y, q.z, q.w);

}
