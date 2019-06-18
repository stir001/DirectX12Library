struct GSOutput
{
	float4 pos : SV_POSITION;
};

#include "CameraLightcBuffer.hlsl"

#include "MatrixOperation.hlsli"

MULTI_CAMERA(b0)

#define VERTEX_NUM (3U)

struct PriVSOutput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct PriGSOut
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    uint viewIndex : SV_ViewportArrayIndex;
};

[maxvertexcount(MAX_CAMERA_NUM * VERTEX_NUM)]
void PrimitiveGS(in triangle PriVSOutput vertices[VERTEX_NUM], inout TriangleStream<PriGSOut> gsOut)
{
    uint i = 0;
    uint j = 0;

    float4x4 pvw = identity();
    PriGSOut gsVert;
    PriVSOutput vsout;
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
            gsVert.normal = vsout.normal;
            gsVert.color = vsout.color;
            gsVert.uv = vsout.uv;
            gsVert.viewIndex = i;
            gsOut.Append(gsVert);
        }
        gsOut.RestartStrip();
    }
}