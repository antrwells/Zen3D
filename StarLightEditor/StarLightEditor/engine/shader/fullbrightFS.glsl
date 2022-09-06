#version 330 core


in vec3 oUv;
in vec4 oCol;


uniform sampler2D tCol;


out vec4 color;

void main(){


    vec4 fc = texture(tCol, oUv.xy).rgba;


    //fc.b = 1;
    //vec4 fcc;
    //fcc.rgb = fc;
    //fcc.a = 1.0;



    //fcc.rgb = oUv;


    color = fc * oCol;

}