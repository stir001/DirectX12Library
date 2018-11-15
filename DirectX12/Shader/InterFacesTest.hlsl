interface ILighting
{
    float4 GetColor(float4 pos, float3 normal);
};

interface ISampling
{
    float4 SamplingColor(float2 uv);
};

class ParallelLight : ILighting
{
    float4 m_dir;
    float4 GetColor(float4 pos, float3 normal)
    {
        return dot(normal.xyz, m_dir.xyz);
    }
};

class PointLight : ILighting
{
    float4 m_pos;
    float4 GetColor(float4 pos, float3 normal)
    {
        return dot(normal.xyz, normalize((m_pos - pos).xyz));
    }
};

class DiffuseSampling : ISampling
{
	float4 SamplingColor(float2 uv)
    {
		return float4(0,1,0,1);
    }
};

cbuffer LightingClassBuffer : register(b0)
{
    ParallelLight lightClass;
    PointLight pointClass;
}

ILighting g_lightingClass;

float4 vs(float4 pos : POSITION) : SV_Position
{
    return pos;
}

float4 ps(float4 pos : POSITION) : SV_Target
{
    return g_lightingClass.GetColor(pos, float3(0, 0, 1));
}