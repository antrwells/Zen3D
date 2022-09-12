
#include "structures.fxh"
#include "RayUtils.fxh"

ConstantBuffer<CubeAttribs>  g_CubeAttribsCB;


struct sVertex {
	float3 position;



	/// <summary>
	/// The color of the vertex.
	/// </summary>
	float4 color;

	/// <summary>
/// The texture coord used.
/// </summary>
	float3 texture_coord;

	/// <summary>
	/// the 3D normal of the vertex.
	/// </summary>
	float3 normal;

	/// <summary>
	/// the Bi-Normal of the vertex.
	/// </summary>
	float3 bi_normal;

	/// <summary>
	/// The tangent of the vertex.
	/// </summary>
	float3 tangent;

	};

struct sTri {

		uint v0;
		uint v1;
		uint v2;

	};

    
struct sGeoIndex {

	uint start_tri;

};


StructuredBuffer<sVertex> bVertex       : register(t1);
StructuredBuffer<sTri> bTri     : register(t2);
StructuredBuffer<sGeoIndex> bGeo : register(t3);



Texture2D    g_CubeTextures[NUM_TEXTURES];
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



    // Calculate texture coordinates.
    float2 uv = bVertex[primitive.x].texture_coord.xy * barycentrics.x +
                bVertex[primitive.y].texture_coord.xy * barycentrics.y +
                bVertex[primitive.z].texture_coord.xy * barycentrics.z;

    // Calculate and transform triangle normal.
    float3 normal = bVertex[primitive.x].normal * barycentrics.x +
                    bVertex[primitive.y].normal * barycentrics.y +
                    bVertex[primitive.z].normal * barycentrics.z;
    normal        = normalize(mul((float3x3) ObjectToWorld3x4(), normal));

    // Sample texturing. Ray tracing shaders don't support LOD calculation, so we must specify LOD and apply filtering.
    payload.Color = g_CubeTextures[NonUniformResourceIndex(InstanceID())].SampleLevel(g_SamLinearWrap, uv, 0).rgb;
    payload.Depth = RayTCurrent();

    // Apply lighting.
    float3 rayOrigin = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();
    LightingPass(payload.Color, rayOrigin, normal, payload.Recursion + 1);
}
