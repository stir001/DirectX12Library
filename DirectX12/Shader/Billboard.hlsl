#define BILLBOARDRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)"\
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

#include "CameraLightcBuffer.hlsl"

#include "MatrixOperation.hlsli"

SamplerState texsampler : register(s0);

Texture2D<float4> colortex : register(t0);

CAMERA_CBUFFER(b0)

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
    float4 svpos : SV_Position;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
};

matrix ExcludeRotation(matrix mat)
{
    matrix rtn = identity();
    rtn._14_24_34 = mat._14_24_34;
    rtn._11 = sqrt(dot((mat._11_12_13), (mat._11_12_13)));
    rtn._22 = sqrt(dot((mat._21_22_23), (mat._21_22_23)));
    rtn._33 = sqrt(dot((mat._31_32_33), (mat._31_32_33)));
    return rtn;
}

[RootSignature(BILLBOARDRS)]
VSOutput BillboardVS(VSInput vInput)
{
    VSOutput o;
    float4x4 notRotaMat = ExcludeRotation(imageMatrix);

    float4x4 notRotaWorld = ExcludeRotation(c_world);

    matrix inverseView = inverse(c_view);

    o.svpos = mul(notRotaMat, mul(notRotaWorld, mul(inverseView, vInput.pos)));
    o.svpos = mul(c_projection, mul(c_view, mul(c_world, o.svpos)));

    o.normal = mul(inverseView, vInput.normal);
    o.uv = vInput.uv;
    o.gamma = vInput.gamma;
    return o;
}

float4 BillboardPS(VSOutput vsout) : SV_Target
{
    //return float4(1, 0, 0, 1);
    return pow(colortex.Sample(texsampler, vsout.uv), vsout.gamma);
}