struct VSOutput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint viewIndex : SV_ViewportArrayIndex;
};

#include "CameraLightcBuffer.hlsl"

#include "MatrixOperation.hlsli"

MULTI_CAMERA(b0)

#define VERTEX_NUM (3U)

[maxvertexcount(MAX_CAMERA_NUM * VERTEX_NUM)]
void FbxGS(in triangle VSOutput vertices[VERTEX_NUM], inout TriangleStream<GSOutput> gsOut)
{
    uint i = 0;
    uint j = 0;

    float4x4 pvw = identity();
    GSOutput gsVert;
    VSOutput vsout;
	[unroll(MAX_CAMERA_NUM)]
    for (i = 0; i < cameraNum; ++i)
    {
        pvw = mul(cameras[i].c_projection, mul(cameras[i].c_view, cameras[i].c_world));
		[unroll(VERTEX_NUM)]
        for (j = 0; j < VERTEX_NUM; ++j)
        {
            vsout = vertices[j];
            gsVert.svpos = mul(pvw, vsout.pos);
            gsVert.pos = vsout.pos;
            gsVert.normal = mul(cameras[i].c_world, vsout.normal);
            gsVert.uv = vsout.uv;
            gsVert.viewIndex = i;
            gsOut.Append(gsVert);
        }
        gsOut.RestartStrip();
    }
}