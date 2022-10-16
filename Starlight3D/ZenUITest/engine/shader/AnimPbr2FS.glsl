#version 330 core
#extension GL_NV_shadow_samplers_cube : enable

uniform sampler2D tCol;
uniform sampler2D tNorm;
uniform samplerCube tShadow;
uniform sampler2D tSpec;
uniform vec3 light_position;
uniform vec3 view_position;
uniform vec3 lDiff;
uniform vec3 lSpec;

uniform float lRange;
in vec2 v_UV;
in vec3 v_Position;
in mat3 v_TBN;
out vec4 _Color;
uniform int shadowMapping;

uniform float lightDepth;


    vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   



void main()
{
    vec3 light_color = vec3(1.f, 1.f, 1.f);
    float abbient_K = 0.6f;
    float specular_K = 0.9f;
    float specular_P = 32f;
    // Ambient.
    vec3 ambient = abbient_K * light_color;
    
    // Diffuse.
    vec3 N = vec3(texture(tNorm, v_UV));
    N = N * 2.0 - 1.0;
    N = normalize(v_TBN * N);
    vec3 light_dir = normalize(light_position - v_Position);
    float diff = max(dot(N, light_dir), 0.f);
    vec3 diffuse = diff * light_color;
    
    // Specular.
    vec3 view_dir = normalize(view_position - v_Position);
    vec3 reflect_dir = reflect(-light_dir, N);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.f), specular_P);
    vec3 specular = specular_K * spec * light_color;
    
    diffuse = diffuse * lDiff;
    specular = specular * lSpec;

    vec3 object_color = vec3(texture(tCol, v_UV));
    vec3 color = (diffuse + specular) * object_color;
    //

       float shadow = 0.0;

    if(shadowMapping==1){
    float bias = 0.15f;
    int samples = 18;
    float viewDistance = length(view_position - v_Position);
    float diskRadius = 0.003f;
    vec3 fragToLight = v_Position - light_position;
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



    _Color = vec4((diffuse * object_color)*shadow, 1.f);
}