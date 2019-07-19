#include "FbxRootsiganatureDefine.hlsli"

#define EX_DEFINE ", DescriptorTable(SRV(t11), visibility = SHADER_VISIBILITY_PIXEL)" \
		", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
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

[RootSignature(FBXRS EX_DEFINE)]
VSOutput FbxVS(VSInput input)
{
    VSOutput o;
    o.pos = input.pos;
    o.uv = input.uv;
    o.normal = float4(input.normal.xyz, 0);

    return o;
}