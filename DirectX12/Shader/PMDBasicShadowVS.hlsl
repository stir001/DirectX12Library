#define PMDRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b2), visibility = SHADER_VISIBILITY_ALL)" \
    ", DescriptorTable(CBV(b3), visibility = SHADER_VISIBILITY_ALL)" \
    ", DescriptorTable(CBV(b4), visibility = SHADER_VISIBILITY_ALL)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	

#define SMP ", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

#define SHADOWTEX ", DescriptorTable(SRV(t1), visibility = SHADER_VISIBILITY_PIXEL)" \

struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    min16uint2 boneno : BONENO;
    min16uint weight : WEIGHT;
};

struct VSOutput
{
    float4 pos : POSITION0;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
};

cbuffer bone : register(b2)
{
    matrix bones[256];
}

cbuffer aMatrix : register(b3)
{
    float4x4 modelMatrix;
}

[RootSignature(PMDRS SHADOWTEX SMP)]
VSOutput BasicShadowVS(VSInput vInput)
{
    float wgt1 = (float) vInput.weight / 100.0;
    float wgt2 = 1.0 - wgt1;
    VSOutput o;
    matrix m = mul(modelMatrix, bones[vInput.boneno[0]] * wgt1 + bones[vInput.boneno[1]] * wgt2);
    o.pos = mul(m, float4(vInput.pos, 1));
    matrix n = m;
    n._m03_m13_m23 = 0;

    o.normal = normalize(mul(n, float4(vInput.normal, 1)));
    o.uv = vInput.uv;
    return o;
}