#include "data/rt/sl_funcs.fxh"

Texture2D    g_Textures[NUM_TEXTURES];
Texture2D    g_TexturesNorm[NUM_TEXTURES];
SamplerState g_SamLinearWrap;

[shader("closesthit")]
void main(inout PrimaryRayPayload payload, in BuiltInTriangleIntersectionAttributes attr)
{

  //  int start_vert = g_CubeAttribsCB.Props[InstanceID()].x;
  

   // uint g_index = GeometryIndex();

    // Calculate triangle barycentrics.
    float3 barycentrics = float3(1.0 - attr.barycentrics.x - attr.barycentrics.y, attr.barycentrics.x, attr.barycentrics.y);

    // Get vertex indices for primitive.
    uint3 primitive;
    
    int start_tri = bGeo[InstanceID()].start_tri;

    

    primitive.x = bTri[start_tri+PrimitiveIndex()].v0;
    primitive.y = bTri[start_tri+PrimitiveIndex()].v1;
    primitive.z = bTri[start_tri+PrimitiveIndex()].v2;

    float3 vert_pos = bVertex[primitive.x].position * barycentrics.x +
                      bVertex[primitive.y].position * barycentrics.y +
                      bVertex[primitive.z].position * barycentrics.z;
    //float3 t_pos        = (mul((float3x3) ObjectToWorld3x4(),vert_pos));



    float3 t_pos = mul(bGeo[InstanceID()].g_Model,vert_pos).xyz;

  float3x3 normalMatrix = (float3x3)bGeo[InstanceID()].g_Model;

    // Calculate texture coordinates.
    float2 uv = bVertex[primitive.x].texture_coord.xy * barycentrics.x +
                bVertex[primitive.y].texture_coord.xy * barycentrics.y +
                bVertex[primitive.z].texture_coord.xy * barycentrics.z;

    // Calculate and transform triangle normal.
    float3 normal = bVertex[primitive.x].normal * barycentrics.x +
                    bVertex[primitive.y].normal * barycentrics.y +
                    bVertex[primitive.z].normal * barycentrics.z;
    normal        = normalize(mul((float3x3) normalMatrix, normal));

    //Tang;
    float3 tang = bVertex[primitive.x].tangent * barycentrics.x +
                    bVertex[primitive.y].tangent * barycentrics.y +
                    bVertex[primitive.z].tangent * barycentrics.z;

     tang         = normalize(mul((float3x3) normalMatrix, tang));


    // Sample texturing. Ray tracing shaders don't support LOD calculation, so we must specify LOD and apply filtering.
   
 payload.Depth = RayTCurrent();
 
    float3 tc;
    tc = g_Textures[NonUniformResourceIndex(InstanceID())].SampleLevel(g_SamLinearWrap, uv, 0).rgb;
    float3 tex_norm = g_TexturesNorm[NonUniformResourceIndex(InstanceID())].SampleLevel(g_SamLinearWrap,uv,0).rgb;

    tex_norm = normalize(tex_norm * 2.0 - 1.0);
    tex_norm.y = -tex_norm.y;





    payload.Color = tc;  

  


   

    float3 T = normalize(mul(tang, normalMatrix));
    float3 N = normalize(mul(normal, normalMatrix));

    T = normalize(T - dot(T, N) * N);

    float3 B = cross(N, T);

    float3x3 TBN = transpose(float3x3(T, B, N));

   
 
  




  

    // Apply lighting.
    float3 rayOrigin = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();
    
    
    LightingPass(payload.Color, rayOrigin, tex_norm,normal,TBN,t_pos, payload.Recursion + 1);
}
