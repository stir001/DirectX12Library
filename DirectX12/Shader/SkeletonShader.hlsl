#define FBXSKRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b2), visibility = SHADER_VISIBILITY_ALL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT" \
	", addressU = TEXTURE_ADDRESS_CLAMP, addressV = TEXTURE_ADDRESS_CLAMP, addressW = TEXTURE_ADDRESS_CLAMP)"

#include "CameraLightcBuffer.hlsl"

CAMERA_CBUFFER(b0)

cbuffer affinematrix : register(b1)
{
    float4x4 modelMatrix;
}

cbuffer color : register(b2)
{
    float4 boneColor;
}

struct VSInput
{
    float4 pos : POSITION;
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
};

[RootSignature(FBXSKRS)]
VSOutput SklVS(VSInput input)
{
    VSOutput vso;
    vso.svpos = mul(c_projection, mul(c_view, mul(c_world, mul(modelMatrix, input.pos))));
    return vso;
}

float4 SklPS(VSOutput vso) : SV_Target
{
    return boneColor;
}