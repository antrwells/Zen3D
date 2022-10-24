
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
    float4x4 bones[100];
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
    float4 camExt : POSITION8;


};

float4x4 inverse(float4x4 m) {
    float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    float idet = 1.0f / det;

    float4x4 ret;


    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;

}

#define IDENTITY_MATRIX float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
#define IDENTITY_MATRIX2 float4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
// Note that if separate shader objects are not supported (this is only the case for old GLES3.0 devices), vertex
// shader output variable name must match exactly the name of the pixel shader input variable.
// If the variable has structure type (like in this example), the structure declarations must also be identical.
void main(in  VSInput VSIn,
    out PSInput PSIn)
{

    float4x4 S = IDENTITY_MATRIX2;
    for (int i = 0; i < 4; ++i)
    {
        if ((int)VSIn.m_BoneIds[i] >= 0)
        {
            S += (bones[(int)VSIn.m_BoneIds[i]]* VSIn.m_Weights[i]);
        }
    }

    float3x3 S_ = transpose((float3x3)inverse(S));



    float4 worldPosition = mul(float4(VSIn.Pos,1.0), g_Model);
    float4 viewPosition = mul(worldPosition, g_View);

 
    float4 a_pos =mul(float4(VSIn.Pos, 1.0),S);
    PSIn.Pos =mul(a_pos,g_MVP);


    //PSIn.Pos = mul(float4(VSIn.Pos, 1.0), g_MVP);

    float3 fragPos = mul(float4(VSIn.Pos,1.0),S).xyz;
  //fragPos = mul(fragPos,g_ModelInv);
    //vec3 T = normalize(normalMatrix * vTan);
    //vec3 N = normalize(normalMatrix * vNorm);
    
    float3x3 normalMatrix = (float3x3)g_Model;

    PSIn.localNormal = normalize(mul(VSIn.Norm, normalMatrix));

    
    float3 T = normalize(mul(VSIn.Tang, S_));
    float3 N = normalize(mul(VSIn.Norm, S_));

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
    PSIn.lPos = lPos.xyz;
    PSIn.vPos = vPos.xyz;
    PSIn.lightProp = lProp;
    PSIn.fragPos = fragPos;
    PSIn.lDiff = lightDiff;
    PSIn.lSpec = lightSpec;
    PSIn.renderProps = renderProps;
    PSIn.camExt = camExt;
    
}