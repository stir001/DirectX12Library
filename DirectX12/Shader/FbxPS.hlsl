Texture2D<float4> diffuseMap : register(t0);
Texture2D<float> diffuseFactorMap : register(t1);
Texture2D<float4> ambientMap : register(t2);
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

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint viewIndex : SV_ViewportArrayIndex;
};

float4 FbxPS(GSOutput output) : SV_Target
{
    float bright = dot(output.normal.xyz, -tailPos.xyz);
    float4 diffuse = (diffuseMap.Sample(smp, output.uv)) * diffuseFactorMap.Sample(smp, output.uv);
    float3 color = saturate(diffuse * bright).rgb;
    float4 ambient = diffuse * ambientMap.Sample(smp, output.uv) * ambinetFactorMap.Sample(smp, output.uv);
    color = saturate(color - ambient.rbg) + ambient.rgb;
    float4 specular = specularMap.Sample(smp, output.uv)
		* pow(
				max(0.0f,
					dot(normalize
						(reflect(-tailPos, output.normal)), -normalize((output.pos - cameras[output.viewIndex].eye))
						)
					), shininessMap.Sample(smp, output.uv)
				)
		* specularFactorMap.Sample(smp, output.uv);
    color += saturate(specular).rgb;
    color += diffuse.rgb * emissiveMap.Sample(smp, output.uv).rgb * emissiveFactorMap.Sample(smp, output.uv);
    float alpha = diffuse.a * transparencyFactorMap.Sample(smp, output.uv);
    return float4(saturate(color), alpha);
}