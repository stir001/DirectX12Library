#include "stdafx.h"
#include "FbxNonShadeRootSignature.h"
#include "Shader/ShaderCompiler.h"

FbxNonShadeRootSignature::FbxNonShadeRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(
		ShaderCompiler::Instance().GetShaderDirPath() + "FbxShader.hlsl",
		"FbxVS",
		"FbxNonShadePS",
		"FbxGS",
		"",
		"",
		true);

	CreateRootSignature("FbxNonShadeRootSignature", mShader.rootSignature.Get(), dev);
}


FbxNonShadeRootSignature::~FbxNonShadeRootSignature()
{
}
