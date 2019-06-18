Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

LIGHT_CBUFFER(b1)

struct PriGSOut
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    uint viewIndex : SV_ViewportArrayIndex;
};

float4 PrimitivePS(PriGSOut data) : SV_Target
{
    float4 color = float4((tex.Sample(smp, data.uv)).rgb * 0.5f + data.color.rgb * 0.5f, data.color.a);
    return saturate(float4(color.xyz * dot(data.normal, float4(-tailPos.xyz, 1)) + color.rgb * 0.2f + data.color.rgb * 0.2f, color.a));
}