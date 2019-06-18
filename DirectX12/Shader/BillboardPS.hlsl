SamplerState texsampler : register(s0);

Texture2D<float4> colortex : register(t0);

struct GSOutput
{
    float4 svpos : SV_Position;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
    uint viewIndex : SV_ViewportArrayIndex;
};

float4 BillboardPS(GSOutput gsout) : SV_Target
{
    return pow(colortex.Sample(texsampler, gsout.uv), gsout.gamma);
}