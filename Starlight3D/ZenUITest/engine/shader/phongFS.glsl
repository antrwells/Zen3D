#version 450
#extension GL_NV_shadow_samplers_cube : enable
#extension GL_NV_bindless_texture : require 
#extension GL_NV_gpu_shader5 : require

uniform vec3 lPos;  
uniform vec3 lDiff;
uniform vec3 lSpec;
uniform float lRange;
uniform sampler2D tCol;
uniform sampler2D tNorm;
uniform sampler2D tSpec;
uniform sampler2D tDisp;
uniform samplerCube tEnv;
uniform samplerCube tShadow;
//


uniform float lightDepth;
uniform vec3 viewVec;
uniform int shadowMapping;
uniform int envMapping;
uniform int refract;
uniform int displace;
uniform float height_scale;
//


uniform vec3 viewPos;

in vec3 Normal; 
in vec3 FragPos;  
in vec3 mUV;
in mat3 mTBN;
in vec3 TLP;
in vec3 TVP;
in vec3 TFP;

    vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    const float minLayers = 8.0;
const float maxLayers = 32.0;
float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * height_scale; 
    vec2 deltaTexCoords = P / numLayers;  

    vec2  currentTexCoords     = texCoords;
float currentDepthMapValue = texture(tDisp, currentTexCoords).r;
  
while(currentLayerDepth < currentDepthMapValue)
{
    // shift texture coordinates along direction of P
    currentTexCoords -= deltaTexCoords;
    // get depthmap value at current texture coordinates
    currentDepthMapValue = texture(tDisp, currentTexCoords).r;  
    // get depth of next layer
    currentLayerDepth += layerDepth;  
}

//return currentTexCoords;
// get texture coordinates before collision (reverse operations)
vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

// get depth after and before collision for linear interpolation
float afterDepth  = currentDepthMapValue - currentLayerDepth;
float beforeDepth = texture(tDisp, prevTexCoords).r - currentLayerDepth + layerDepth;
 
// interpolation of texture coordinates
float weight = afterDepth / (afterDepth - beforeDepth);
vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

return finalTexCoords;  

} 

out vec4 FragColor;

void main()
{

    vec2 texCoords;
    vec3 viewDir = normalize(TVP-TFP);
    if(displace==1){

       
        texCoords = ParallaxMapping(mUV.xy,  viewDir);

    
    }else{
        texCoords = mUV.xy;
    }


    

    vec3 color = texture(tCol, texCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;




    vec3 normal = texture(tNorm,texCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);   

    //normal = normalize(mTBN * normal); 

    vec3 lightDir = normalize(TLP - TFP);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    diffuse *= lDiff;

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec;
    specular *= lSpec;
    specular *= texture2D(tSpec,texCoords).rgb;
    vec4 final_col = vec4(ambient + diffuse + specular, 1.0);



  


    
    float shadow;

    if(shadowMapping==1){
    float bias = 0.15f;
    int samples = 18;
    float viewDistance = length(viewPos - FragPos);
    float diskRadius = 0.003f;
    vec3 fragToLight = FragPos - lPos;
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

    final_col.rgb *= vec3(shadow,shadow,shadow);

    FragColor = final_col;


 

}