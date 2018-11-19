#define IMAGE3DRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)"\
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

#include "CameraLightcBuffer.hlsl"

#include "MatrixOperation.hlsli"

SamplerState texsampler : register(s0);

Texture2D<float4> colortex : register(t0);

MULTI_CAMERA(b0)

cbuffer imageMatrix : register(b1)
{
    float4x4 imageMatrix;
}

struct VSInput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
};

struct VSOutput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
};

struct GSOutput
{
    float4 svpos : SV_Position;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
    uint viewIndex : SV_ViewportArrayIndex;
};

[RootSignature(IMAGE3DRS)]
VSOutput Image3DVS(VSInput input)
{
    VSOutput output;
    output.pos = mul(imageMatrix, input.pos);
    output.normal = mul(imageMatrix, input.normal);
    output.uv = input.uv;
    output.gamma = input.gamma;

    return output;
}

float4 Image3DPS(GSOutput gsout) : SV_Target
{
    return pow(colortex.Sample(texsampler, gsout.uv), gsout.gamma);
}

#define VERTEX_COUNT 12U

[maxvertexcount(VERTEX_COUNT)]
void Image3DGS(in triangle VSOutput vertices[3], inout TriangleStream<GSOutput> gsOut)
{
    uint i = 0;
    uint j = 0;

    float4x4 pvw = identity();
    GSOutput gsVert;
    VSOutput vsout;
	[unroll(4)]
    for (i = 0; i < cameraNum; ++i)
    {
        pvw = mul(cameras[i].c_projection, mul(cameras[i].c_view, cameras[i].c_world));
		[unroll(3)]
        for (j = 0; j < 3; ++j)
        {
            vsout = vertices[j];
            gsVert.svpos = mul(pvw, vsout.pos);
            gsVert.normal = mul(cameras[i].c_world, vsout.normal);
            gsVert.uv = vsout.uv;
            gsVert.gamma = vsout.gamma;
            gsVert.viewIndex = i;
            gsOut.Append(gsVert);
        }
        gsOut.RestartStrip();
    }
}