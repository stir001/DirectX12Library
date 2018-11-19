#define PMDRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b2), visibility = SHADER_VISIBILITY_ALL)" \
    ", DescriptorTable(CBV(b3), visibility = SHADER_VISIBILITY_ALL)" \
    ", DescriptorTable(CBV(b4), visibility = SHADER_VISIBILITY_ALL)" \

#include "CameraLightcBuffer.hlsl"

#include "MatrixOperation.hlsli"

MULTI_CAMERA(b0)

LIGHT_CBUFFER(b1)

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
    float specularity;
    float alpha;
}

struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    min16uint2 boneno : BONENO;
    min16uint weight : WEIGHT;
};

struct VSOutput {
	float4 pos : POSITION0;
	float4 normal : NORMAL;
	float2 uv :TEXCOORD;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION0;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint viewIndex : SV_ViewportArrayIndex;
};

//頂点しぇーだー
[RootSignature(PMDRS)]
VSOutput BasicVS(VSInput vInput)
{
    float wgt1 = (float)vInput.weight / 100.0;
	float wgt2 = 1.0 - wgt1;
    VSOutput o;
    matrix m = mul(modelMatrix, bones[vInput.boneno[0]] * wgt1 + bones[vInput.boneno[1]] * wgt2);
    o.pos = mul(m, float4(vInput.pos, 1));
    //o.pos = mul(c_projection, mul(c_view, mul(c_world, mul(m, float4(vInput.pos, 1)))));
    //o.shadowpos = mul(mul(c_projection, mul(c_view, c_world)), float4(vInput.pos, 1));
    matrix n =  m;
    n._m03_m13_m23 = 0;

    o.normal = normalize(mul(n, float4(vInput.normal, 1)));
    o.uv = vInput.uv;
	return o;
}

//ピクセルシェーダー
float4 BasicPS(GSOutput data) : SV_Target
{
	float4 light = dir;
	float brightness = dot(data.normal.xyz,-light.xyz);
    float4 vray = float4(data.pos.xyz - cameras[data.viewIndex].eye.xyz, 1);
    vray = float4(normalize(vray.xyz), 1);
    float spec = saturate(pow(max(0.0f,dot(normalize(reflect(-light.xyz, data.normal.xyz)), -vray.xyz)), specularity));
    float4 color = diffuse * brightness + (specular * spec) + ambient;

    return float4(color.xyz, alpha);
}

#define VERTEX_COUNT 12U

[maxvertexcount(VERTEX_COUNT)]
void PmdGS(in triangle VSOutput vertices[3], inout TriangleStream<GSOutput> gsOut)
{
    uint i = 0;
    uint j = 0;

    float4x4 pvw = identity();
    GSOutput gsVert;
    VSOutput vsout;
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
            gsVert.normal = mul(cameras[i].c_world, vsout.normal);
            gsVert.uv = vsout.uv;
            gsVert.viewIndex = i;
            gsOut.Append(gsVert);
        }
        gsOut.RestartStrip();
    }
}
