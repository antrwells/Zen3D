#include "sl_structs.fxh"

ShadowRayPayload CastShadow(RayDesc ray, uint Recursion)
{
    // By default initialize Shading with 0.
    // With RAY_FLAG_SKIP_CLOSEST_HIT_SHADER, only intersection and any-hit shaders are executed.
    // Any-hit shader is not used in this tutorial, intersection shader can not write to payload, 
    // so on intersection payload. Shading is always 0,
    // on miss shader payload.Shading will be initialized with 1.
    // With this flags shadow casting executed as fast as possible.
    ShadowRayPayload payload = {0.0, Recursion};
    
    // Manually terminate the recusrion as the driver doesn't check the recursion depth.
    if (Recursion >= bScene[0].maxRecursion)
    {
        payload.Shading = 1.0;
        return payload;
    }
    TraceRay(g_TLAS,            // Acceleration structure
             RAY_FLAG_FORCE_OPAQUE | RAY_FLAG_SKIP_CLOSEST_HIT_SHADER | RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH,
             OPAQUE_GEOM_MASK,  // Instance inclusion mask - only opaque instances are visible
             SHADOW_RAY_INDEX,  // Ray contribution to hit group index (aka ray type)
             HIT_GROUP_STRIDE,  // Multiplier for geometry contribution to hit 
                                // group index (aka the number of ray types)
             SHADOW_RAY_INDEX,  // Miss shader index
             ray,
             payload);
    return payload;
}


// Calculate perpendicular to specified direction.
void GetRayPerpendicular(float3 dir, out float3 outLeft, out float3 outUp)
{
    const float3 a    = abs(dir);
    const float2 c    = float2(1.0, 0.0);
    const float3 axis = a.x < a.y ? (a.x < a.z ? c.xyy : c.yyx) :
                                    (a.y < a.z ? c.xyx : c.yyx);
    outLeft = normalize(cross(dir, axis));
    outUp   = normalize(cross(dir, outLeft));
}

// Returns a ray within a cone.
float3 DirectionWithinCone(float3 dir, float2 offset)
{
    float3 left, up;
    GetRayPerpendicular(dir, left, up);
    return normalize(dir + left * offset.x + up * offset.y);
}

// Calculate lighting.
void LightingPass(inout float3 Color, float3 Pos,float3 tex_norm, float3 Norm,float3x3 TBN,float3 fragPos, uint Recursion)
{
    RayDesc ray;
    float3  col = float3(0.0, 0.0, 0.0);

    // Add a small offset to avoid self-intersections.
    ray.Origin = Pos + Norm * SMALL_OFFSET;
    ray.TMin   = 0.0;

    for (int i = 0; i < bScene[0].num_lights; ++i)
    {
        // Limit max ray length by distance to light source.
        ray.TMax = distance(bScene[0].lightPos[i].xyz, Pos) * 1.01;

        float3 rayDir = normalize(bScene[0].lightPos[i].xyz - Pos);
        float  NdotL   = max(0.0, dot(Norm, rayDir));

     
          float3 TLP = mul(bScene[0].lightPos[i].xyz, TBN);
        float3 TVP = mul(bScene[0].CameraPos.xyz, TBN);
          float3 TFP = mul(fragPos, TBN);

         float3 lightDir = normalize(TLP - TFP);

        float diff = max(dot(lightDir, tex_norm),0.0);

        //dist

         float xd = bScene[0].lightPos[i].x -fragPos.x;
    float yd = bScene[0].lightPos[i].y - fragPos.y;
    float zd = bScene[0].lightPos[i].z - fragPos.z;

    float dis = sqrt(xd * xd + yd * yd + zd * zd);

    if (dis < 0) {
        dis = -dis;
    }

   

    float dv = dis / bScene[0].lightRange[i];

    if (dv > 1.0) {
        dv = 1.0;
    }
    dv = 1.0 - dv;

    dis = dv;



        NdotL = diff;
        // Optimization - don't trace rays if NdotL is zero or negative
        if (NdotL > 0.0)
        {
            // Cast multiple rays that are distributed within a cone.
            int   PCFSamples = Recursion > 1 ? min(1, bScene[0].ShadowPCF) : bScene[0].ShadowPCF;
            float shading    = 0.0;
            for (int j = 0; j < PCFSamples; ++j)
            {
                float2 offset = float2(bScene[0].DiscPoints[j / 2][(j % 2) * 2], bScene[0].DiscPoints[j / 2][(j % 2) * 2 + 1]);
                ray.Direction = DirectionWithinCone(rayDir, offset * 0.005);
                shading       += saturate(CastShadow(ray, Recursion).Shading);
            }
            
            shading = PCFSamples > 0 ? shading / float(PCFSamples) : 1.0;

            col += (Color * bScene[0].lightDiff[i].rgb * NdotL * shading)*dis;
        }
        //col += Color * 0.125;
    }
    Color = col;// * (1.0 / float(bScene[0].num_lights));//+ g_ConstantsCB.AmbientColor.rgb;
}
