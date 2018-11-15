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

CAMERA_CBUFFER(b0)

LIGHT_CBUFFER(b1)

cbuffer affinematrix: register(b2)
{
   float4x4 modelMatrix;
}

struct Input
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    //float4x4 vmatrix : VERTEXMATRIX;
};

struct Output
{
    float4 svpos : SV_POSITION;
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

[RootSignature(FBXRS SMAPLWEDEFINE)]
Output FbxVS(Input input)
{
    Output o;
    o.pos = mul(c_world, mul(modelMatrix, input.pos));
    o.svpos = mul(c_projection, mul(c_view,  o.pos));
    o.uv = input.uv;

    matrix m = mul(c_world, modelMatrix);
    o.normal = normalize(mul(m, float4(input.normal.xyz, 0)));

    return o;
}

float4 FbxPS(Output output) : SV_Target
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
						(reflect(-dir, output.normal)), -normalize((output.pos - eye))
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

PSOutput FbxGeometryPS(Output input)
{
    PSOutput output;
    float2 viewNorm = mul(c_view,input.normal).xy;
    viewNorm = PackingNormal(viewNorm);//法線パッキング
    output.normal = viewNorm;
    output.albedo = (diffuseMap.Sample(smp, input.uv.xy));
    output.specular = specularMap.Sample(smp, input.uv.xy);

    return output;
}
