#include "FbxRootsiganatureDefine.hlsli"

#define SMAPLERDEFINE ", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_CLAMP, addressV = TEXTURE_ADDRESS_CLAMP, addressW = TEXTURE_ADDRESS_CLAMP)"

struct VSInput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
};

cbuffer affinematrix : register(b2)
{
    float4x4 modelMatrix;
}

[RootSignature(FBXRS SMAPLERDEFINE)]
VSOutput FbxVS(VSInput input)
{
    VSOutput o;
    o.pos = mul(modelMatrix, input.pos);
    o.uv = input.uv;
    o.normal = normalize(mul(modelMatrix, float4(input.normal.xyz, 0)));

    return o;
}