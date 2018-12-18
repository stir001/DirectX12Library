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
    float4 pos : SV_Position;
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
    //float4x4 notRotaMat = ExcludeRotation(imageMatrix);

    //float4x4 notRotaWorld = ExcludeRotation(c_world);

    //matrix inverseView = inverse(c_view);

    //o.svpos = mul(notRotaMat, mul(notRotaWorld, mul(inverseView, vInput.pos)));
    //o.svpos = mul(c_projection, mul(c_view, mul(c_world, o.svpos)));
    o.pos = vInput.pos;

    o.normal = vInput.normal;
    o.uv = vInput.uv;
    o.gamma = vInput.gamma;
    return o;
}

float4 BillboardPS(GSOutput gsout) : SV_Target
{
    return pow(colortex.Sample(texsampler, gsout.uv), gsout.gamma);
}

#define VERTEX_COUNT 12U

[maxvertexcount(VERTEX_COUNT)]
void BillboardGS(in triangle VSOutput vertices[3], inout TriangleStream<GSOutput> gsOut)
{
    uint i = 0;
    uint j = 0;

    float4x4 pvw = identity();
    float4x4 invMat = identity();
    GSOutput gsVert;
    VSOutput vsout;
	[unroll(4)]
    for (i = 0; i < cameraNum; ++i)
    {
        pvw = mul(cameras[i].c_projection, mul(cameras[i].c_view, cameras[i].c_world));
        invMat = inverse(cameras[i].c_view);
        invMat._41_42_43 = invMat._14_24_34 = 0.0f;
        invMat = mul(ExcludeRotation(imageMatrix), mul(ExcludeRotation(cameras[i].c_world), invMat));
		[unroll(3)]
        for (j = 0; j < 3; ++j)
        {
            vsout = vertices[j];
            gsVert.svpos = mul(pvw, mul(invMat, vsout.pos));
            gsVert.normal = mul(cameras[i].c_world, mul(ExcludeRotation(imageMatrix), vsout.normal));
            gsVert.uv = vsout.uv;
            gsVert.gamma = vsout.gamma;
            gsVert.viewIndex = i;
            gsOut.Append(gsVert);
        }
        gsOut.RestartStrip();
    }
}