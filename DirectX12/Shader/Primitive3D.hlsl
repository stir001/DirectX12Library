#define PRM3DRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

#include "MatrixOperation.hlsli"

MULTI_CAMERA(b0)

LIGHT_CBUFFER(b1)

struct PriVSInput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    matrix aMat : INSTANCEMAT;
    float4 instanceOffset : INSTANCEPOS;
    uint instanceID : SV_InstanceID;
};

struct PriVSOutput
{
    float4 pos : POSITION0;
    float4 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct PriGSOut
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION0;
    float4 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    uint viewIndex : SV_ViewportArrayIndex;
};


[RootSignature(PRM3DRS)]
PriVSOutput PrimitiveVS(PriVSInput vsInput)
{
    PriVSOutput po;
    po.pos = mul(vsInput.aMat, vsInput.pos) + vsInput.instanceOffset;
    po.color = vsInput.color;
	po.uv = vsInput.uv;
    matrix rotaMat = vsInput.aMat;
    rotaMat._14_24_34 = 0;
    po.normal = mul(rotaMat, vsInput.normal);
    return po;
}

float4 PrimitivePS(PriGSOut data) : SV_Target
{
    float4 color = (tex.Sample(smp, data.uv)) * 0.5f;
    return saturate(float4(color * dot(data.normal, float4(-dir.xyz, 1)) + color * 0.2f));
}

#define VERTEX_COUNT 12U

[maxvertexcount(VERTEX_COUNT)]
void PrimitiveGS(in triangle PriVSOutput vertices[3], inout TriangleStream<PriGSOut> gsOut)
{
    uint i = 0;
    uint j = 0;

    float4x4 pvw = identity();
    PriGSOut gsVert;
    PriVSOutput vsout;
	[unroll(4)]
    for (i = 0; i < cameraNum; ++i)
    {
        pvw = mul(cameras[i].c_projection, mul(cameras[i].c_view, cameras[i].c_world));
		[unroll(3)]
        for (j = 0; j < 3; ++j)
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