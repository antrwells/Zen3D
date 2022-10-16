#version 460
layout (local_size_x = 1, local_size_y = 1) in;
layout (rgba32f, binding = 0) uniform readonly lowp image2D srcTex;
layout (rgba32f, binding = 1) uniform writeonly lowp image2D destTex;
void main() {
  ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
  vec4 color = imageLoad(srcTex, pixel_coords); 

 vec3 fc = vec3(0,0,0);

 ivec2 new_coords = pixel_coords;

    int samples=0;

  for(int y=-8;y<8;y++){
      for(int x=-8;x<8;x++){

          new_coords.x = pixel_coords.x + x*3;
          new_coords.y = pixel_coords.y + y*3;

          fc.rgb += imageLoad(srcTex,new_coords).rgb;
            samples++;

      }
  }

  //samples = samples/2;
  fc.rgb /= samples;

    color.rgb = fc;

  imageStore(destTex, pixel_coords, color);
}