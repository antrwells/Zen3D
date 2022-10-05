#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vP;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec4 vCol;

uniform mat4 proj;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec4 col;

void main(){

	UV = vUV;
	col = vCol;
	gl_Position = proj * vec4(vP,1);
	
}



