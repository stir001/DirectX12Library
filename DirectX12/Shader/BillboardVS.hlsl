#define BILLBOARDRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)"\
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

struct VSInput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
};

[RootSignature(BILLBOARDRS)]
VSOutput BillboardVS(VSInput vInput)
{
    VSOutput o;
    o.pos = vInput.pos;
    o.normal = vInput.normal;
    o.uv = vInput.uv;
    o.gamma = vInput.gamma;
    return o;
}