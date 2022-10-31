#version 450
#extension GL_NV_bindless_texture : require 
#extension GL_NV_gpu_shader5 : require


// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec4 vCol;
layout(location = 2) in vec3 vUv;
layout(location = 3) in vec3 vNorm;
layout(location = 4) in vec3 vBiNorm;
layout(location = 5) in vec3 vTan;
layout(location = 6) in ivec4 in_BoneIds;
layout(location = 7) in vec4 in_Weights;

uniform mat4 mProj;
uniform mat4 mModel;
uniform mat4 mView;
uniform vec3 viewPos;
uniform vec3 lPos;
uniform int displace;


out vec3 Normal;
out vec3 Position;
out vec3 FragPos;  
out vec3 mUV;
out mat3 mTBN;
out vec3 TLP;
out vec3 TVP;
out vec3 TFP;

void main(){

   vec3 T = normalize(vec3(mModel * vec4(vTan,   0.0)));

   vec3 N = normalize(vec3(mModel * vec4(vNorm,    0.0)));

   T = normalize(T - dot(T, N) * N);

    vec3 B;

if(displace==1){
   B = cross(N, T);
}else{
    B = cross(N,T);
}

    mat3 TBN = transpose(mat3(T, B, N));  

    TLP = TBN * lPos;
    TVP = TBN * viewPos;
    TFP = TBN * vec3(mModel * vec4(vPos,1.0));


    mUV = vUv;
    gl_Position = mProj * mView * mModel * vec4(vPos, 1.0);
    FragPos = vec3(mModel * vec4(vPos, 1.0));
    Normal = mat3(transpose(inverse(mModel))) * vNorm;  


}