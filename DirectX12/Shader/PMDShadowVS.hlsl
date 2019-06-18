#define SHADOWRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b2), visibility = SHADER_VISIBILITY_ALL)" \
    ", DescriptorTable(CBV(b3), visibility = SHADER_VISIBILITY_ALL)" \
    ", DescriptorTable(CBV(b4), visibility = SHADER_VISIBILITY_ALL)" \

cbuffer bone : register(b2)
{
    matrix bones[256];
}

cbuffer aMatrix : register(b3)
{
    float4x4 modelMatrix;
}

struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    min16uint2 boneno : BONENO;
    min16uint weight : WEIGHT;
};

struct VSOutput
{
    float4 pos : POSITION0;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
};

[RootSignature(SHADOWRS)]
float4 ShadowVS(VSInput vInput) : SV_Position
{
    float wgt1 = (float) vInput.weight / 100.0;
    float wgt2 = 1.0 - wgt1;
    float4 svpos;
    matrix m = mul(modelMatrix, bones[vInput.boneno[0]] * wgt1 + bones[vInput.boneno[1]] * wgt2);
    svpos = mul(m, float4(vInput.pos, 1));
  
    return svpos;
}