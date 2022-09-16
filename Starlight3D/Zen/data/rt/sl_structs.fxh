RaytracingAccelerationStructure g_TLAS;
struct PrimaryRayPayload
{
    float3 Color;
    float  Depth;
    uint   Recursion;
};

struct ShadowRayPayload
{
    float  Shading;   // 0 - fully shadowed, 1 - fully in light, 0..1 - for semi-transparent objects
    uint   Recursion; // Current recusrsion depth
};

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

    float4 m_BoneIds;

    float4 m_Weights;

	};

struct sTri {

		uint v0;
		uint v1;
		uint v2;

	};

    
struct sGeoIndex {

	uint start_tri;
  float4x4 g_Model;
  float4x4 g_ModelInv;

};

struct sSceneInfo{

  uint num_lights;
  float3 lightPos[32];
  float3 lightDiff[32];
  float3 lightSpec[32];
  float lightRange[32];
  float3 camPos;
  float camMinZ;
  float camMaxZ;
   float4   CameraPos;
   float4x4 InvViewProj;
    uint maxRecursion;
 uint ShadowPCF;
	float4  DiscPoints[8];


};

StructuredBuffer<sVertex> bVertex       : register(t1);
StructuredBuffer<sTri> bTri     : register(t2);
StructuredBuffer<sGeoIndex> bGeo : register(t3);
StructuredBuffer<sSceneInfo> bScene : register(t4);

// Ray types
#define HIT_GROUP_STRIDE  2
#define PRIMARY_RAY_INDEX 0
#define SHADOW_RAY_INDEX  1

// Instance mask.
#define OPAQUE_GEOM_MASK      0x01
#define TRANSPARENT_GEOM_MASK 0x02
#ifndef __cplusplus

// Small offset between ray intersection and new ray origin to avoid self-intersections.
#    define SMALL_OFFSET 0.0001

// For procedural intersections you must add custom hit kind.
#    define RAY_KIND_PROCEDURAL_FRONT_FACE 1
#    define RAY_KIND_PROCEDURAL_BACK_FACE  2

#endif


PrimaryRayPayload CastPrimaryRay(RayDesc ray, uint Recursion)
{
    PrimaryRayPayload payload = {float3(0, 0, 0), 0.0, Recursion};

    // Manually terminate the recusrion as the driver doesn't check the recursion depth.
    if (Recursion >= bScene[0].maxRecursion)
    {
        // set pink color for debugging
        payload.Color = float3(0.95, 0.18, 0.95);
        return payload;
    }
    TraceRay(g_TLAS,            // Acceleration structure
             RAY_FLAG_NONE,
             ~0,                // Instance inclusion mask - all instances are visible
             PRIMARY_RAY_INDEX, // Ray contribution to hit group index (aka ray type)
             HIT_GROUP_STRIDE,  // Multiplier for geometry contribution to hit
                                // group index (aka the number of ray types)
             PRIMARY_RAY_INDEX, // Miss shader index
             ray,
             payload);
    return payload;
}

