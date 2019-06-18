#define IMGRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
	", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_MIP_POINT"   \
    ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"


struct VSInput
{
    float4 pos : POSITION;
    float gamma : GAMMA;
    uint vertexID : VERTEXID;
    float2 uv[4] : TEXCOORD;
    float4x4 mat : INSTANCEMAT;
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
};

[RootSignature(IMGRS)]
VSOutput ImageVS(VSInput input)
{
    VSOutput o;
    o.svpos = mul(input.mat, input.pos);
    o.uv = input.uv[input.vertexID];
    o.gamma = input.gamma;
    return o;
}