#include "stdafx.h"
#include "PrimitiveNormalMapRootSignature.h"
#include "Shader/ShaderCompiler.h"

PrimitiveNormalMapRootSignature::PrimitiveNormalMapRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(ShaderCompiler::Instance().GetShaderDirPath() + "NormalmapShader.hlsl", "NormalMapVS", "NormalMapPS", "NormalMapGS", "", "", true);
	CreateRootSignature("NormalMapRootSignature",mShader.rootSignature.Get(), dev);
}

PrimitiveNormalMapRootSignature::~PrimitiveNormalMapRootSignature()
{
}
