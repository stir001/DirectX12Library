Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION0;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint viewIndex : SV_ViewportArrayIndex;
};

#include "CameraLightcBuffer.hlsl"

MULTI_CAMERA(b0)

LIGHT_CBUFFER(b1)

cbuffer mat : register(b4)
{
    float4 diffuse;
    float4 specular;
    float4 ambient;
    float alpha;
    float specularity;
}

float4 BasicPS(GSOutput data) : SV_Target
{
    float4 color = tex.Sample(smp, data.uv);
    float4 light = tailPos;
    float4 vray = float4(data.pos.xyz - cameras[data.viewIndex].eye.xyz, 1);
    vray = float4(normalize(vray.xyz), 1);
    float spec = saturate(pow(max(0.0f, dot(normalize(reflect(-light.xyz, data.normal.xyz)), -vray.xyz)), specularity));
    return saturate(color * dot(data.normal.xyz, -tailPos.xyz) + color * ambient + specular * spec);
}