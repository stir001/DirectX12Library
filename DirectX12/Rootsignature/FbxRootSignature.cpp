#include "stdafx.h"
#include "FbxRootSignature.h"
#include "Shader/ShaderCompiler.h"


FbxRootSignature::FbxRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(
		ShaderCompiler::Instance().GetShaderDirPath() + "FbxShader.hlsl",
		"FbxVS",
		"FbxPS",
		"FbxGS",
		"",
		"",
		true);

	CreateRootSignature("FbxRootSignature", mShader.rootSignature.Get(), dev);
}


FbxRootSignature::~FbxRootSignature()
{
}
