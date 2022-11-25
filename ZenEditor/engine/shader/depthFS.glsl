#version 330 core

uniform vec3 mCol;
uniform vec3 camP;
uniform float minZ;
uniform float maxZ;




in vec3 fVert;

out vec4 color;

void main(){

  	float dis = length(fVert-camP);

	dis = dis / maxZ;

	if(dis<0){
		dis=0;
	}
	if(dis>1){
		dis = 1;
	}

    vec4 fc;


    fc.rgb = vec3(dis,dis,dis);
    fc.a = 1.0;

    color = fc;


}