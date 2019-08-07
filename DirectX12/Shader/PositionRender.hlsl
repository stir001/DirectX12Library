#define PRM3DRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	

#define SMP ", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

#include "MatrixOperation.hlsli"

struct VSInput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    matrix aMat : INSTANCEMAT;
    float4 instanceOffset : INSTANCEPOS;
    float4 color : INSTANCECOLOR;
    uint instanceID : SV_InstanceID;
};

struct VSOutput
{
    float4 pos : POSITION;
};

struct GSOutput
{
    float4 pos : POSITION;
    float4 svpos : SV_POSITION;
    uint viewIndex : SV_ViewportArrayIndex;
};

#define VIEW_PORT_NUM (6U)

cbuffer ViewPortCamera
{
    matrix vp[VIEW_PORT_NUM];
}

[RootSignature(PRM3DRS SMP)]
VSOutput PrimitiveVS(VSInput vsInput)
{
    VSOutput po;
    po.pos = vsInput.pos;
    return po;
}

#define VERTEX_NUM (3U)

[maxvertexcount(VIEW_PORT_NUM * VERTEX_NUM)]
void PrimitiveGS(in triangle VSOutput vertices[VERTEX_NUM], inout TriangleStream<GSOutput> gsOut)
{
    uint i = 0;
    uint j = 0;

    float4x4 pvw = identity();
    GSOutput gsVert;
    VSOutput vsout;
	[unroll(VIEW_PORT_NUM)]
    for (i = 0; i < VIEW_PORT_NUM; ++i)
    {
		[unroll(VERTEX_NUM)]
        for (j = 0; j < VERTEX_NUM; ++j)
        {
            vsout = vertices[j];
            gsVert.svpos = mul(vp[i], vsout.pos);
            gsVert.pos = vsout.pos;
            gsVert.viewIndex = i;
            gsOut.Append(gsVert);
        }
        gsOut.RestartStrip();
    }
}

float4 main(VSOutput vsOut) : SV_TARGET
{
	return vsOut.pos;
}