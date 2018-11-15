#define IMAGE3DRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)"\
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

#include "CameraLightcBuffer.hlsl"

SamplerState texsampler : register(s0);

Texture2D<float4> colortex : register(t0);

CAMERA_CBUFFER(b0)

cbuffer imageMatrix : register(b1)
{
    float4x4 imageMatrix;
}

struct VSinput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
};

struct VSoutput
{
    float4 svpos : SV_Position;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
};

[RootSignature(IMAGE3DRS)]
VSoutput Image3DVS(VSinput input)
{
    VSoutput output;
    output.svpos = mul(c_projection, mul(c_view, mul(c_world, mul(imageMatrix, input.pos))));
    output.normal = mul(imageMatrix, input.normal);
    output.uv = input.uv;
    output.gamma = input.gamma;

    return output;
}

float4 Image3DPS(VSoutput vsout):SV_Target
{
    //return float4(1, 0, 0, 1);
    return pow(colortex.Sample(texsampler, vsout.uv), vsout.gamma);
}