Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
};

float4 ImagePS(VSOutput input) : SV_Target
{
    return pow(abs(tex.Sample(smp, input.uv)), input.gamma);
}