#define PMDTOONEXRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b2), visibility = SHADER_VISIBILITY_ALL)" \
    ", DescriptorTable(CBV(b3), visibility = SHADER_VISIBILITY_ALL)" \
    ", DescriptorTable(CBV(b4), visibility = SHADER_VISIBILITY_ALL)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
    ", DescriptorTable(SRV(t1), visibility = SHADER_VISIBILITY_PIXEL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

Texture2D<float4> tex : register(t0);
Texture2D<float4> toon : register(t1);

SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

#include "MatrixOperation.hlsli"

MULTI_CAMERA(CAMERA_REGISTER)

LIGHT_CBUFFER(LIGHT_REGISTER)

cbuffer bone : register(b2)
{
    matrix bones[256];
}

cbuffer aMatrix : register(b3)
{
    float4x4 modelMatrix;
}

cbuffer mat : register(b4)
{
    float4 diffuse;
    float4 specular;
    float4 ambient;
    float alpha;
    float specularity;
}

struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    min16uint2 boneno : BONENO;
    min16uint weight : WEIGHT;
};

struct VSOutput
{
    float4 pos : POSITION0;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION0;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint viewIndex : SV_ViewportArrayIndex;
};

[RootSignature(PMDTOONEXRS)]
VSOutput PmdToonVS(VSInput vInput)
{
    float wgt1 = (float) vInput.weight / 100.0;
    float wgt2 = 1.0 - wgt1;
    VSOutput o;
    matrix m = mul(modelMatrix, bones[vInput.boneno[0]] * wgt1 + bones[vInput.boneno[1]] * wgt2);
    o.pos = mul(m, float4(vInput.pos, 1));
    matrix n = m;
    n._m03_m13_m23 = 0;

    o.normal = normalize(mul(n, float4(vInput.normal, 1)));
    o.uv = vInput.uv;
    return o;
}

float4 PmdToonPS(GSOutput data) : SV_Target
{
    float4 color = tex.Sample(smp, data.uv);
    float4 light = dir;
    float lambert = abs(dot(data.normal.xyz, -dir.xyz));
    float4 vray = float4(data.pos.xyz - cameras[data.viewIndex].eye.xyz, 1);
    vray = float4(normalize(vray.xyz), 1);
    float spec = saturate(pow(max(0.0f, dot(normalize(reflect(-light.xyz, data.normal.xyz)), -vray.xyz)), specularity));
    return saturate(color * toon.Sample(smp, float2(0, lambert)) /*+ color * ambient*/ + specular * spec);
}

#define VERTEX_NUM (3U)

[maxvertexcount(MAX_CAMERA_NUM * VERTEX_NUM)]
void PmdGS(in triangle VSOutput vertices[VERTEX_NUM], inout TriangleStream<GSOutput> gsOut)
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