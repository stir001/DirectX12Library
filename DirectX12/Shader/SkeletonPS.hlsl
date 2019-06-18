struct VSOutput
{
    float4 svpos : SV_POSITION;
};

cbuffer color : register(b2)
{
    float4 boneColor;
}

float4 SklPS(VSOutput vso) : SV_Target
{
    return boneColor;
}