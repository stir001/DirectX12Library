#define PRM2DRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_MIP_POINT"   \
    ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

SamplerState smp : register(s0);

struct Output
{
    float4 pos : POSITION;
    float4 svpos : SV_POSITION;
    float4 color : COLOR;
};

[RootSignature(PRM2DRS)]
Output Primitive2DVS(float3 pos : POSITION, float3 color : COLOR)
{
    Output o;
    o.pos = float4(pos, 1);
    o.svpos = o.pos;
    o.color = float4(color, 1);

    return o;
}

float4 Primitive2DPS (Output input): SV_Target
{
    return input.color;
}