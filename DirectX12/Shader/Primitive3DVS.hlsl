#define PRM3DRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	

#define SMP ", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

struct PriVSInput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    matrix aMat : INSTANCEMAT;
    float4 instanceOffset : INSTANCEPOS;
    float4 color : INSTANCECOLOR;
    uint instanceID : SV_InstanceID;
};

struct PriVSOutput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

[RootSignature(PRM3DRS SMP)]
PriVSOutput PrimitiveVS(PriVSInput vsInput)
{
    PriVSOutput po;
    po.pos = mul(vsInput.aMat, vsInput.pos) + float4(vsInput.instanceOffset.xyz, 0);
    po.color = vsInput.color;
    po.uv = vsInput.uv;
    matrix rotaMat = vsInput.aMat;
    rotaMat._14_24_34 = 0;
    po.normal = mul(rotaMat, vsInput.normal);
    return po;
}