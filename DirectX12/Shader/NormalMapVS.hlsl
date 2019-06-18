#define PRM3DRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

struct NormalMapData
{
    float4 pos : POSITION0;
    float4 normal : NORMAL;
    matrix aMatrix : MATRIX;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct NormalMapVSInput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    matrix aMat : INSTANCEMAT;
    float4 instanceOffset : INSTANCEPOS;
    float4 color : INSTANCECOLOR;
    uint instanceID : SV_InstanceID;
};

[RootSignature(PRM3DRS)]
NormalMapData NormalMapVS(NormalMapVSInput vsIn)
{
    NormalMapData data;
    data.pos = mul(vsIn.aMat, vsIn.pos) + float4((vsIn.instanceOffset).xyz, 0.0f);
    data.color = vsIn.color;
    data.uv = vsIn.uv;
    matrix rotaMat = vsIn.aMat;
    rotaMat._14_24_34 = 0;
    rotaMat._41_42_43 = 0;
    data.normal = vsIn.normal;
    data.aMatrix = rotaMat;
    return data;
}