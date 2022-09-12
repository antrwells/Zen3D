
#include "data/rt/sl_structs.fxh"



[shader("miss")]
void main(inout PrimaryRayPayload payload)
{
  
    payload.Color = float3(0,0,0);
 //   //payload.Depth = RayTCurrent(); // bug in DXC for SPIRV
    payload.Depth = bScene[0].camMinZ;
}
