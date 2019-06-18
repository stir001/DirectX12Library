#define IMAGE3DRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
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
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float gamma : GAMMA;
};

cbuffer imageMatrix : register(b1)
{
    float4x4 imageMatrix;
}

[RootSignature(IMAGE3DRS)]
VSOutput Image3DVS(VSInput input)
{
    VSOutput output;
    output.pos = mul(imageMatrix, input.pos);
    output.normal = mul(imageMatrix, input.normal);
    output.uv = input.uv;
    output.gamma = input.gamma;

    return output;
}