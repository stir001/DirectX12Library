struct GSOutput
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION0;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint viewIndex : SV_ViewportArrayIndex;
};

Texture2D<float4> tex : register(t0);
Texture2D<float> shadowmap : register(t1);
SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

MULTI_CAMERA(CAMERA_REGISTER)

LIGHT_CBUFFER(LIGHT_REGISTER)

float4 BasicShadowPS(GSOutput data) : SV_Target
{
    float4 color = tex.Sample(smp, data.uv);
    float4 light = tailPos;
    float4 vray = float4(data.pos.xyz - cameras[data.viewIndex].eye.xyz, 1);
    vray = float4(normalize(vray.xyz), 1);
    float spec = saturate(pow(max(0.0f, dot(normalize(reflect(-light.xyz, data.normal.xyz)), -vray.xyz)), specularity));

    float3 shadowpos = mul(lightviewProj, data.pos).xyz;
    shadowpos.xy = (shadowpos.xy + 1.0f) * 0.5f;
    float shadow = shadowmap.Sample(smp, shadowpos.xy) < shadowpos.z ? 0.5f : 1.0f;

    return saturate(color * dot(data.normal.xyz, -tailPos.xyz) + color * ambient + specular * spec) * shadow;
}