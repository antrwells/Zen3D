#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec4 vCol;
layout(location = 2) in vec3 vUv;
layout(location = 3) in vec3 vNorm;
layout(location = 4) in vec3 vBiNorm;
layout(location = 5) in vec3 vTan;
layout(location = 6) in ivec4 in_BoneIds;
layout(location = 7) in vec4 in_Weights;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 bone_transforms[100];

uniform mat4 mProj;
uniform mat4 mModel;
uniform mat4 mView;
out vec3 fVert;


void main(){
    
  mat4 S = mat4(0.f);
    for (int i = 0; i < 4; ++i)
    {
        if (in_BoneIds[i] >= 0)
        {
            S += (bone_transforms[in_BoneIds[i]] * in_Weights[i]);
        }
    }
    mat3 S_ = transpose(inverse(mat3(S)));
    mat4 MVP = mProj * mView * mModel;
    gl_Position = MVP * S * vec4(vPos, 1.f);
  
    fVert = vec3(mModel * S * vec4(vPos, 1.f));
    

    //vec3 T = normalize(S_ * vTan);
    //vec3 B = normalize(S_ * vBiNorm);
    //vec3 N = normalize(S_ * vNorm);

    //v_TBN = mat3(T, B, N);


}