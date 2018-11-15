interface ILighting
{
    float4 GetColor(float3 normal);
};

class ParallelLight : ILighting
{
    float4 dir;
    float4 GetColor(float3 normal);
};

cbuffer LightingClassBuffer : register(b0)
{
    ILighting lightClass;
}

float4 ps(float4 pos : POSITION) : SV_Position
{

}