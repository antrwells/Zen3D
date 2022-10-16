
cbuffer Constants
{
    float4x4 g_MVP;
    float4x4 g_Proj;
    float4x4 g_Model;
    float4x4 g_View;
    float4 viewPos;
    float4 camExt;
 
};

// Vertex shader takes two inputs: vertex position and color.
// By convention, Diligent Engine expects vertex shader inputs to be 
// labeled 'ATTRIBn', where n is the attribute number.
struct VSInput
{
    float3 Pos   : ATTRIB0;
    float4 Color : ATTRIB1;
    float3 Uv : ATTRIB2;
    float3 Norm : ATTRIB3;
    float3 BiNorm : ATTRIB4;
    float3 Tang : ATTRIB5;
    float4 m_BoneIds : ATTRIB6;
    float4 m_Weights : ATTRIB7;
};

struct PSInput
{
    float4 Pos   : SV_POSITION;
    float3 vPos : POSITION1;
    float3 fragPos : POSITION2;
    float4 camExt : POSITION3;

};

// Note that if separate shader objects are not supported (this is only the case for old GLES3.0 devices), vertex
// shader output variable name must match exactly the name of the pixel shader input variable.
// If the variable has structure type (like in this example), the structure declarations must also be identical.
void main(in  VSInput VSIn,
    out PSInput PSIn)
{

    float4 worldPosition = mul(float4(VSIn.Pos,1.0), g_Model);
    float4 viewPosition = mul(worldPosition, g_View);
    PSIn.Pos = mul(float4(VSIn.Pos, 1.0), g_MVP);

    float depth = PSIn.Pos.z / PSIn.Pos.w;


    float3 fragPos = mul(float4(VSIn.Pos,1.0),g_Model);
    
    PSIn.vPos = viewPos.xyz;
    PSIn.fragPos = fragPos;
    PSIn.camExt = camExt;
    
   
}