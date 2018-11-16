#define PRM3DRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

MULTI_CAMERA(b0)

LIGHT_CBUFFER(b1)

struct PriOutput
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION0;
    float4 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct PriVSInput
{
    float4 pos : POSITION; 
	float4 normal : NORMAL; 
	float4 color : COLOR; 
    float2 uv : TEXCOORD;
    matrix aMat : INSTANCEMAT;
    float4 instanceOffset : INSTANCEPOS;
    uint instanceID : SV_InstanceID;
};

    [RootSignature(PRM3DRS)]
PriOutput PrimitiveVS(PriVSInput vsInput)
{
    PriOutput po;
    matrix pvw = mul(cameras[0].c_projection, mul(cameras[0].c_view, cameras[0].c_world));
    po.svpos = mul(pvw, mul(vsInput.aMat, vsInput.pos)) + mul(pvw, vsInput.instanceOffset);
    po.pos = po.svpos;
    po.color = vsInput.color;
	po.uv = vsInput.uv;
    matrix rotaMat = vsInput.aMat;
    rotaMat._14_24_34 = 0;
    po.normal = mul(rotaMat, vsInput.normal);
    return po;
}

float4 PrimitivePS(PriOutput data) : SV_Target
{
    float4 color = (tex.Sample(smp, data.uv)/* + data.color*/) * 0.5f;
    return saturate(float4(color * dot(data.normal, float4(-dir.xyz, 1)) + color * 0.2f));
}