#define WIRERS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT" \
	", addressU = TEXTURE_ADDRESS_CLAMP, addressV = TEXTURE_ADDRESS_CLAMP, addressW = TEXTURE_ADDRESS_CLAMP)"

#include "CameraLightcBuffer.hlsl"

#include "MatrixOperation.hlsli"

MULTI_CAMERA(b0)

struct VSInput
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct GSOutput
{
    float4 svpos : SV_Position;
    float4 color : COLOR;
    uint viewIndex : SV_ViewportArrayIndex;
};

[RootSignature(WIRERS)]
VSOutput WireVS(VSInput vIn)
{
    VSOutput vOut;
    vOut.pos = vIn.pos;
    vOut.color = vIn.color;
    return vOut;
}

float4 WirePS(GSOutput pIn) : SV_target
{
    return pIn.color;
}

#define INPUT_VERTEX_NUM (2U)

[maxvertexcount(MAX_CAMERA_NUM * INPUT_VERTEX_NUM)]
void WireGS(in line VSOutput vertices[INPUT_VERTEX_NUM], inout LineStream<GSOutput> gsOut)
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
		[unroll(INPUT_VERTEX_NUM)]
        for (j = 0; j < INPUT_VERTEX_NUM; ++j)
        {
            vsout = vertices[j];
            gsVert.svpos = mul(pvw, vsout.pos);
            gsVert.color = vsout.color;
            gsVert.viewIndex = i;
            gsOut.Append(gsVert);
        }
        gsOut.RestartStrip();
    }
}