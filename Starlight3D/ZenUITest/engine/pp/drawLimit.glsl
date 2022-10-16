#version 330 core

in vec2 UV;
in vec4 col;

out vec4 color;

uniform sampler2D tR;
uniform float limit;

void main(){

    vec4 co = texture(tR,UV);

    float brightness = dot(co.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > limit)
        color = vec4(co.rgb, 1.0);
    else
        color = vec4(0.0, 0.0, 0.0, 1.0);


    
}