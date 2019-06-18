Texture2D<float4> normalmap : register(t0);
SamplerState smp : register(s0);

struct PrimitiveVertexData
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 tangentLight : LIGHT;
    float4 eyeDir : EYEDIR;
    matrix uvzMatrix : UVZMAT;
    matrix localMat : INSTANCEMAT;
};

float4 NormalMapPS(PrimitiveVertexData psIn) : SV_target
{
    float4 normal = float4(normalize(((normalmap.Sample(smp, psIn.uv)) * 2.0f - 1.0f).xyz), 1);

    float brightness = (dot(-psIn.tangentLight.xyz, normalize(normal.xyz)) + 1.0f) * 0.5f;

    float3 refLight = normalize(reflect(normalize(psIn.tangentLight.xyz), normalize(normal.xyz)));
    float spec = pow(saturate(dot(normalize(refLight), -psIn.eyeDir.xyz)), 5);
    //return float4(spec, spec, spec, 1.0f);

    float4 color = saturate(float4((psIn.color * brightness + saturate(float4(1, 1, 1, 0) * spec)).xyz, 1.0f));
    return color;
}