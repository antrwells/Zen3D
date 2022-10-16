#version 450
#extension GL_NV_bindless_texture : require 
#extension GL_NV_gpu_shader5 : require
#extension GL_NV_shadow_samplers_cube : enable

flat in int texID;
in vec3 oUv;
in vec4 oCol;
in vec3 oFragPos;
in vec3 TLP;
in vec3 TVP;
in vec3 TFP;
in vec3 rPos;
in vec3 oNorm;
in vec3 reflectVector;
in vec3 pass_normal;
in mat3 normMat;
in mat3 TBN;



uniform samplerCube tShadow;

uniform vec3 lPos;
uniform vec3 lDiff;
uniform vec3 lSpec;
uniform float lRange;
uniform vec3 viewPos;
uniform float lightDepth;
uniform vec3 viewVec;

uniform SamplersNV { uint64_t texMaps[1024]; };

uniform int shadowMapping;

    vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   


out vec4 color;

void main(){


    int index=0;

    index =  texID;

    vec3 col = texture(sampler2D(texMaps[index]),oUv.xy).rgb;

    color.rgb = col;
    color.a = 1.0;

}