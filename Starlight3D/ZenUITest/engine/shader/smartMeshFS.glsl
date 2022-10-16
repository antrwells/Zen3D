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
in ivec4 texStart;


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

    sampler2D tCol = sampler2D(texMaps[texID]);
    sampler2D tNorm = sampler2D(texMaps[texID+1]);
    sampler2D tSpec = sampler2D(texMaps[texID+2]);


     float dis = abs(length(oFragPos-lPos));

    float dv = dis/lRange;

    if(dv>1.0) dv = 1.0;
    dv = 1.0-dv;

    vec3 df = vec3(dv,dv,dv);

    //
    vec2 uv;

    uv.x = oUv.x;
    uv.y = 1.0-oUv.y;
    
    vec3 normal = vec3(0.5,0.5,1);

      normal = texture2D(tNorm,uv).rgb;

    normal = normalize(normal * 2.0 - 1.0);

    vec3 ref_Norm = reflectVector;




    vec3 cTex = texture(tCol,uv).rgb;

    //

     vec3 lightDir = normalize(TLP - TFP);

    float diff = max(dot(lightDir,normal),0.0);

    vec3 diffuse = vec3(diff,diff,diff) * lDiff;

    diffuse = diffuse * cTex;

    vec3 viewDir = normalize(TVP-TFP);
    vec3 reflectDir = reflect(-lightDir,normal);
    vec3 halfwayDir = normalize(lightDir+viewDir);

    float spec = pow(max(dot(normal,halfwayDir),0.0),32.0);

    spec = spec;

    vec3 specular = ((lSpec) * spec); 

    // Shadows

     float cosTheta = abs( dot( normalize(viewVec), pass_normal) );
    float fresnel = pow(1.0 - cosTheta, 4.0);


    float shadow = 0.0;

    if(shadowMapping==1){
    float bias = 0.15f;
    int samples = 18;
    float viewDistance = length(viewPos - oFragPos);
    float diskRadius = 0.003f;
    vec3 fragToLight = oFragPos - lPos;
    float currentDepth = length(fragToLight);
    float ld2 = currentDepth/lRange;
    if(ld2>1.0) ld2 = 1.0;
    ld2 = 1.0 - ld2;
    fragToLight = normalize(fragToLight);
    for(int i=0;i<samples;i++){

        float closestDepth = textureCube(tShadow,fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= lightDepth;
        if((currentDepth - bias) > closestDepth){
            shadow += 1.0;
        }

    }
    shadow /= float(samples);
    shadow = 1.0 - shadow;
    
    }else{
        shadow = 1.0;
    }

    vec4 specv= texture2D(tSpec,uv);

    vec4 fc;

    fc.rgb = ((diffuse + (specular*specv.rgb))*df)*vec3(shadow,shadow,shadow);
    fc.a = 1.0;

   

    color = fc;

}