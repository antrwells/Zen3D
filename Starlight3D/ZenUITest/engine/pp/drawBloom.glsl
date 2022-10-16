#version 330 core

in vec2 UV;
in vec4 col;

out vec4 color;

uniform sampler2D tR;
uniform sampler2D tB;
uniform float bloom;


void main(){

    //vec4 co = texture(tR,UV) * col;
   
    vec3 color_map = texture(tR,UV).rgb;
    vec3 bloom_map = texture(tB,UV).rgb;

    vec4 fcol = vec4(0,0,0,1);

    fcol.rgb = (color_map*0.6) + (bloom_map*0.9);

    color = fcol;

}