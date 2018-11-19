#include "FbxRootsiganatureDefine.hlsli"

#define SMAPLWEDEFINE ", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_CLAMP, addressV = TEXTURE_ADDRESS_CLAMP, addressW = TEXTURE_ADDRESS_CLAMP)"

Texture2D<float4> diffuseMap:register(t0);
Texture2D<float> diffuseFactorMap : register(t1);
Texture2D<float4> ambientMap:register(t2);
Texture2D<float> ambinetFactorMap : register(t3);
Texture2D<float4> specularMap : register(t4);
Texture2D<float> specularFactorMap : register(t5);
Texture2D<float> shininessMap : register(t6);
Texture2D<float4> emissiveMap : register(t7);
Texture2D<float> emissiveFactorMap : register(t8);
Texture2D<float4> transparentColorMap : register(t9);
Texture2D<float> transparencyFactorMap : register(t10);

SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

#include "MatrixOperation.hlsli"

MULTI_CAMERA(b0)

LIGHT_CBUFFER(b1)

cbuffer affinematrix: register(b2)
{
   float4x4 modelMatrix;
}

struct VSInput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    //float4x4 vmatrix : VERTEXMATRIX;
};

struct VSOutput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct PSOutput
{
    float2 normal   : Sv_Target0;
    float4 specular : Sv_Target1;
    float4 albedo   : Sv_Target2;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint viewIndex : SV_ViewportArrayIndex;
};

[RootSignature(FBXRS SMAPLWEDEFINE)]
VSOutput FbxVS(VSInput input)
{
    VSOutput o;
    o.pos = mul(modelMatrix, input.pos);
    o.uv = input.uv;
    o.normal = normalize(mul(modelMatrix, float4(input.normal.xyz, 0)));

    return o;
}

float4 FbxPS(GSOutput output) : SV_Target
{
    float bright = dot(output.normal.xyz, -dir.xyz);
    float4 diffuse = (diffuseMap.Sample(smp, output.uv)) * diffuseFactorMap.Sample(smp, output.uv);
    float4 color = saturate(diffuse * bright);
    float4 ambient = diffuse * ambientMap.Sample(smp, output.uv) * ambinetFactorMap.Sample(smp, output.uv);
    color = saturate(color - ambient) + ambient;
    float4 specular = specularMap.Sample(smp, output.uv) 
		*	pow(
				max(0.0f, 
					dot(normalize
						(reflect(-dir, output.normal)), -normalize((output.pos - cameras[output.viewIndex].eye))
						)
					), shininessMap.Sample(smp, output.uv)
				) 
		* specularFactorMap.Sample(smp, output.uv);
    color += saturate(specular);
    color += diffuse * emissiveMap.Sample(smp, output.uv) * emissiveFactorMap.Sample(smp, output.uv);
    color.a *= transparencyFactorMap.Sample(smp, output.uv).r * transparencyFactorMap.Sample(smp, output.uv);
    return saturate(color);
}

float2 PackingNormal(float2 viewNorm)
{
    return float2(0.5 * (viewNorm.xy + 1.0f));
}

PSOutput FbxGeometryPS(GSOutput input)
{
    PSOutput output;
    float2 viewNorm = mul(cameras[input.viewIndex].c_view,input.normal).xy;
    viewNorm = PackingNormal(viewNorm);//法線パッキング
    output.normal = viewNorm;
    output.albedo = (diffuseMap.Sample(smp, input.uv.xy));
    output.specular = specularMap.Sample(smp, input.uv.xy);

    return output;
}

#define VERTEX_COUNT 12U

[maxvertexcount(VERTEX_COUNT)]
void FbxGS(in triangle VSOutput vertices[3], inout TriangleStream<GSOutput> gsOut)
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
