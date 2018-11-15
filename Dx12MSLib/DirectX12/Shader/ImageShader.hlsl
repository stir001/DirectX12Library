#define IMGRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
	", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_MIP_POINT"   \
    ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"


Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);

struct ImageOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
    float gamma : GAMMA;
};

[RootSignature(IMGRS)]
ImageOutput ImageVS(float4 pos : POSITION, float2 uv : TEXCOORD, float gamma:GAMMA)
{
    ImageOutput o;
	o.svpos = pos;
	o.uv = uv;
    o.gamma = gamma;
	return o;
}

float4 ImagePS(ImageOutput input) : SV_Target
{
    return pow(tex.Sample(smp, input.uv), input.gamma);
}