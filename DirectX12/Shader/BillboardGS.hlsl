#include "MatrixOperation.hlsli"

#include "CameraLightcBuffer.hlsl"

MULTI_CAMERA(b0)

cbuffer imageMatrix : register(b1)
{
    float4x4 imageMatrix;
}

struct GSOutput
{
    float4 svpos : SV_Position;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
    uint viewIndex : SV_ViewportArrayIndex;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
};

#define VERTEX_COUNT 3

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