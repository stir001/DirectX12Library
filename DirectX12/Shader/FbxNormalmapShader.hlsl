#include "FbxRootsiganatureDefine.hlsli"

#define EX_DEFINE ", DescriptorTable(SRV(t11), visibility = SHADER_VISIBILITY_PIXEL)" \
		", StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT"   \
        ", addressU = TEXTURE_ADDRESS_CLAMP, addressV = TEXTURE_ADDRESS_CLAMP, addressW = TEXTURE_ADDRESS_CLAMP)"

Texture2D<float4> diffuseMap:register(t0);
Texture2D<float> diffuseFactorMap : register(t1);
Texture2D<float4> ambientMap:register(t2);
Texture2D<float> ambinetFactorMap : register(t3);
Texture2D<float4> specularMap : register(t4);
Texture2D<float> specularFactorMap : register(t5);
Texture2D<float> shininessMap : register(t6);
Texture2D<float4> emissiveMap : register(t7);
Texture2D<float> emissiveFactorMap : register(t8);
Texture2D<float4> transparentColorMap : register(t9);
Texture2D<float> transparencyFactorMap : register(t10);
Texture2D<float4> normalMap : register(t11);

SamplerState smp : register(s0);

#include "CameraLightcBuffer.hlsl"

#include "MatrixOperation.hlsli"

MULTI_CAMERA(b0)

LIGHT_CBUFFER(b1)

cbuffer affinematrix: register(b2)
{
   float4x4 modelMatrix;
}

struct VSInput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    //float4x4 vmatrix : VERTEXMATRIX;
};

struct VSOutput
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct PSOutput
{
    float2 normal   : Sv_Target0;
    float4 specular : Sv_Target1;
    float4 albedo   : Sv_Target2;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float4 tangentLight : LIGHT;
    float2 uv : TEXCOORD;
    uint viewIndex : SV_ViewportArrayIndex;
};

[RootSignature(FBXRS EX_DEFINE)]
VSOutput FbxVS(VSInput input)
{
    VSOutput o;
    //o.pos = mul(modelMatrix, input.pos);
    o.pos = input.pos;
    o.uv = input.uv;
    o.normal = float4(input.normal.xyz, 0);
    //o.normal = normalize(mul(modelMatrix, float4(input.normal.xyz, 0)));

    return o;
}

float4 FbxPS(GSOutput output) : SV_Target
{
    float4 normal = float4(normalize(((normalMap.Sample(smp, output.uv)) * 2.0f - 1.0f).xyz), 1.0f);
    float bright = dot(normal.xyz, -output.tangentLight.xyz);
    float4 diffuse = (diffuseMap.Sample(smp, output.uv)) * diffuseFactorMap.Sample(smp, output.uv);
    float4 color = saturate(diffuse * bright);
    float4 ambient = diffuse * ambientMap.Sample(smp, output.uv) * ambinetFactorMap.Sample(smp, output.uv);
    color = saturate(color - ambient) + ambient;
    float4 specular = specularMap.Sample(smp, output.uv) 
		*	pow(
				max(0.0f, 
					dot(normalize
						(reflect(-output.tangentLight, normal)), -normalize((output.pos - cameras[output.viewIndex].eye))
						)
					), shininessMap.Sample(smp, output.uv)
				) 
		* specularFactorMap.Sample(smp, output.uv);
    color += saturate(specular);
    color += diffuse * emissiveMap.Sample(smp, output.uv) * emissiveFactorMap.Sample(smp, output.uv);
    color.a *= transparencyFactorMap.Sample(smp, output.uv).r * transparencyFactorMap.Sample(smp, output.uv);
    return saturate(color);
}

float2 PackingNormal(float2 viewNorm)
{
    return float2(0.5 * (viewNorm.xy + 1.0f));
}

PSOutput FbxGeometryPS(GSOutput input)
{
    PSOutput output;
    float2 viewNorm = mul(cameras[input.viewIndex].c_view,input.normal).xy;
    viewNorm = PackingNormal(viewNorm);//法線パッキング
    output.normal = viewNorm;
    output.albedo = (diffuseMap.Sample(smp, input.uv.xy));
    output.specular = specularMap.Sample(smp, input.uv.xy);

    return output;
}

#define VERTEX_NUM (3U)

[maxvertexcount(MAX_CAMERA_NUM * VERTEX_NUM)]
void FbxGS(in triangle VSOutput vertices[VERTEX_NUM], inout TriangleStream<GSOutput> gsOut)
{
    uint i = 0;
    uint j = 0;

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
    matrix localMat = inverse(modelMatrix);
    float4 localLight = mul(localMat, dir);
    float4 tangentLight = float4(normalize(mul(localLight, tMat).xyz), 1.0f);


    float4x4 pvw = identity();
    GSOutput gsVert;
    gsVert.tangentLight = tangentLight;
    VSOutput vsout;
	[unroll(MAX_CAMERA_NUM)]
    for (i = 0; i < cameraNum; ++i)
    {
        pvw = mul(cameras[i].c_projection, mul(cameras[i].c_view, cameras[i].c_world));
		[unroll(VERTEX_NUM)]
        for (j = 0; j < VERTEX_NUM; ++j)
        {
            vsout = vertices[j];
            gsVert.svpos = mul(pvw, mul(modelMatrix, vsout.pos));
            gsVert.pos = vsout.pos;
            gsVert.normal = mul(cameras[i].c_world, vsout.normal);
            gsVert.uv = vsout.uv;
            gsVert.viewIndex = i;
            gsOut.Append(gsVert);
        }
        gsOut.RestartStrip();
    }
}
