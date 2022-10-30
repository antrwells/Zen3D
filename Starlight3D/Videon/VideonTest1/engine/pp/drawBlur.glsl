#version 330 core

in vec2 UV;
in vec4 col;

out vec4 color;

uniform sampler2D tR;
uniform float blur;




void main(){

    //vec4 co = texture(tR,UV) * col;
    vec4 co = vec4(0,0,0,1);

    float cx,cy;

    cx =0;
    cy =0;

  float samples = 0;

    for(cx=-12;cx<12;cx++){
      for(cy=-12;cy<12;cy++){

        vec2 buv = vec2(0,0);
        buv.x = UV.x + cx * blur * 0.01;
        buv.y = UV.y + cy * blur * 0.01;
        if(buv.x<0 || buv.x>1 || buv.y <0 || buv.y>1)
        {
          continue;
        }
        samples=samples+1;

        co.rgb += texture2D(tR,buv).rgb;

      }
    }

    co.rgb /= samples;

  //  co.rgb = vec3(0,0,0);
    co.a = 1.0;

    color = co;

}