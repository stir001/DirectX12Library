#define SKYRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \

	
#define SMP ", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_CLAMP, addressV = TEXTURE_ADDRESS_CLAMP, addressW = TEXTURE_ADDRESS_CLAMP)"

Texture2D<float4> tex : register(t0);

SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

#include "MatrixOperation.hlsli"

MULTI_CAMERA(b0)

struct SkyBoxVSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct SkyBoxVSOutput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct SkyBoxGSOut
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
    uint viewIndex : SV_ViewportArrayIndex;
};

cbuffer SkyBoxCameraProjection : register(b1)
{
    matrix projections[MAX_CAMERA_NUM];
}

[RootSignature(SKYRS SMP)]
SkyBoxVSOutput SkyBoxVS(SkyBoxVSInput vsInput)
{
    SkyBoxVSOutput po;
    po.pos = vsInput.pos;
    po.uv = vsInput.uv;
    return po;
}

float4 SkyBoxPS(SkyBoxGSOut data) : SV_Target
{
   
    return tex.Sample(smp, data.uv);
}

#define VERTEX_NUM (3U)

[maxvertexcount(MAX_CAMERA_NUM * VERTEX_NUM)]
void SkyBoxGS(in triangle SkyBoxVSOutput vertices[VERTEX_NUM], inout TriangleStream<SkyBoxGSOut> gsOut)
{
    uint i = 0;
    uint j = 0;

    float4x4 pvw = identity();
    SkyBoxGSOut gsVert;
    SkyBoxVSOutput vsout;
	[unroll(MAX_CAMERA_NUM)]
    for (i = 0; i < cameraNum; ++i)
    {
        pvw = mul(projections[i], mul(ExcludeTranslation(cameras[i].c_view), cameras[i].c_world));
		[unroll(VERTEX_NUM)]
        for (j = 0; j < VERTEX_NUM; ++j)
        {
            vsout = vertices[j];
            gsVert.svpos = mul(pvw, vsout.pos);
            gsVert.uv = vsout.uv;
            gsVert.viewIndex = i;
            gsOut.Append(gsVert);
        }
        gsOut.RestartStrip();
    }
}