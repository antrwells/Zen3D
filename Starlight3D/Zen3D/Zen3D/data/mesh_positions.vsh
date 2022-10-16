
cbuffer Constants
{
    float4x4 g_MVP;
    float4x4 g_Proj;
    float4x4 g_Model;
    float4x4 g_View;
    float4x4 g_ModelInv;
    float4 vPos;
    float4 lPos;
    float4 lProp;
    float4 lightDiff;
    float4 lightSpec;
    float4 renderProps;
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
    float4 Color : COLOR0;
    float3 Norm : NORMAL0;
    float3 Uv : TEX_COORD;
    float3 TLP : NORMAL1;
    float3 TVP : NORMAL2;
    float3 TFP : NORMAL3;
    float3 lPos : POSITION1;
    float3 vPos : POSITION2;
    float3 lightProp : POSITION3;
    float3 fragPos : POSITION4;
    float3 lDiff : POSITION5;
    float3 lSpec : POSITION6;
    float3 localNormal : NORMAL4;
    float4 renderProps : POSITION7;


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

    float3 fragPos = mul(float4(VSIn.Pos,1), g_Model).xyz;

    //vec3 T = normalize(normalMatrix * vTan);
    //vec3 N = normalize(normalMatrix * vNorm);
    
    float3x3 normalMatrix = (float3x3)g_ModelInv;

    PSIn.localNormal = normalize(mul(VSIn.Norm, normalMatrix));

    
    float3 T = normalize(mul(VSIn.Tang, normalMatrix));
    float3 N = normalize(mul(VSIn.Norm, normalMatrix));

    T = normalize(T - dot(T, N) * N);

    float3 B = cross(N, T);
    
    float3x3 TBN = transpose(float3x3(T, B, N));

    PSIn.TLP = mul(lPos.xyz, TBN);
    PSIn.TVP = mul(vPos.xyz, TBN);
    PSIn.TFP = mul(fragPos, TBN);

    //float4x4 mvp = g_Proj;
    //PSIn.Pos = mul(float4(VSIn.Pos, 1.0),transpose(mvp));
    PSIn.Norm = mul(VSIn.Norm, transpose((float3x3)g_ModelInv));
   // PSIn.Norm = float3(0, 0, 0);
    PSIn.Color = VSIn.Color;
    PSIn.Uv = VSIn.Uv;
    PSIn.lPos = lPos;
    PSIn.vPos = vPos;
    PSIn.lightProp = lProp;
    PSIn.fragPos = fragPos;
    PSIn.lDiff = lightDiff;
    PSIn.lSpec = lightSpec;
    PSIn.renderProps = renderProps;
}