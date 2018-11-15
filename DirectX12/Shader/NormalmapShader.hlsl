#define PRM3DRS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)" \
    ", DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)" \
	", DescriptorTable(CBV(b0), visibility = SHADER_VISIBILITY_ALL)" \
	", DescriptorTable(CBV(b1), visibility = SHADER_VISIBILITY_ALL)" \
	", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_WRAP, addressV = TEXTURE_ADDRESS_WRAP, addressW = TEXTURE_ADDRESS_WRAP)"

#include "MatrixOperation.hlsli"

Texture2D<float4> normalmap : register(t0);
SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

CAMERA_CBUFFER(b0);

LIGHT_CBUFFER(b1)

struct NormalMapData
{
    float4 svpos : SV_POSITION;
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
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    matrix aMat : INSTANCEMAT;
    float4 instanceOffset : INSTANCEPOS;
    uint instanceID : SV_InstanceID;
};

struct PrimitiveVertexData
{
    float4 svpos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float4 tangentLight : LIGHT;
	matrix uvzMatrix : UVZMAT;
	matrix localMat : INSTANCEMAT;
};

[RootSignature(PRM3DRS)]
NormalMapData NormalMapVS(NormalMapVSInput vsIn)
{
    NormalMapData data;
    matrix pvw = mul(c_projection, mul(c_view, c_world));
    data.svpos = ((mul(pvw, mul(vsIn.aMat, vsIn.pos)) + mul(pvw, vsIn.instanceOffset)));
    data.pos = vsIn.pos;
    data.color = vsIn.color;
    data.uv = vsIn.uv;
    matrix rotaMat = vsIn.aMat;
    rotaMat._14_24_34 = 0;
    rotaMat._41_42_43 = 0;
    data.normal = vsIn.normal;
    data.aMatrix = rotaMat;
    return data;
}

#define VERTEX_COUNT 3U

[maxvertexcount(VERTEX_COUNT)]void NormalMapGS(in triangle NormalMapData vertices[VERTEX_COUNT], inout TriangleStream<PrimitiveVertexData> gsOut)
{	
    matrix tangentSpace;
    tangentSpace._14_24_34 = 0.0f;
    tangentSpace._31_32_33 = vertices[0].normal.xyz;
    tangentSpace._41_42_43 = 0.0f;
    tangentSpace._44 = 1.0f;

    float3 deltavXUV01 = float3(vertices[0].pos.x - vertices[1].pos.x, vertices[0].uv - vertices[1].uv);
    float3 deltavXUV03 = float3(vertices[0].pos.x - vertices[2].pos.x, vertices[0].uv - vertices[2].uv);
    float3 delatacrossXUV = cross(deltavXUV01, deltavXUV03);
	
    tangentSpace._11 = -delatacrossXUV.y / delatacrossXUV.x;
    tangentSpace._21 = -delatacrossXUV.z / delatacrossXUV.x;

    float3 deltavYUV01 = float3(vertices[0].pos.y - vertices[1].pos.y, vertices[0].uv - vertices[1].uv);
    float3 deltavYUV03 = float3(vertices[0].pos.y - vertices[2].pos.y, vertices[0].uv - vertices[2].uv);
    float3 delatacrossYUV = cross(deltavYUV01, deltavYUV03);

    tangentSpace._12 = -delatacrossYUV.y / delatacrossYUV.x;
    tangentSpace._22 = -delatacrossYUV.z / delatacrossYUV.x;

    float3 deltavZUV01 = float3(vertices[0].pos.z - vertices[1].pos.z, vertices[0].uv - vertices[1].uv);
    float3 deltavZUV03 = float3(vertices[0].pos.z - vertices[2].pos.z, vertices[0].uv - vertices[2].uv);
    float3 delatacrossZUV = cross(deltavZUV01, deltavZUV03);

    tangentSpace._13 = -delatacrossZUV.y / delatacrossZUV.x;
    tangentSpace._23 = -delatacrossZUV.z / delatacrossZUV.x;

    tangentSpace._11_12_13 = normalize(tangentSpace._11_12_13);
    tangentSpace._21_22_23 = normalize(tangentSpace._21_22_23);
    tangentSpace._31_32_33 = normalize(tangentSpace._31_32_33);

    matrix tMat = transpose(tangentSpace);
    matrix localMat = inverse(vertices[0].aMatrix);
    float4 localLight = mul(localMat, dir);
    float4 tangentLight = float4(normalize(mul(localLight, tMat).xyz), 1.0f);

    PrimitiveVertexData ret;
    ret.color = vertices[0].color;
    ret.tangentLight = float4(normalize(tangentLight.xyz), 1.0);
    ret.uvzMatrix = tangentSpace;
    ret.localMat = vertices[0].aMatrix;
    for (uint i = 0; i < VERTEX_COUNT; ++i)
    {
        ret.svpos = vertices[i].svpos;
        ret.uv = vertices[i].uv;
        gsOut.Append(ret);
    }
    gsOut.RestartStrip();
}

float4 NormalMapPS(PrimitiveVertexData psIn) : SV_target
{
    float4 normal = float4(normalize(((normalmap.Sample(smp, psIn.uv)) * 2.0f - 1.0f).xyz), 1);

    float brightness = (dot(-psIn.tangentLight.xyz, normalize(normal.xyz)) + 1.0f) * 0.5f;

    float4 color = saturate(float4((psIn.color * brightness).xyz, 1.0f));
    return color;
}