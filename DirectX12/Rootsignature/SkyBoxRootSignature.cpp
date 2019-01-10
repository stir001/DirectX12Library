#include "stdafx.h"
#include "SkyBoxRootSignature.h"
#include "Shader/ShaderCompiler.h"

SkyBoxRootSignature::SkyBoxRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(ShaderCompiler::Instance().GetShaderDirPath() + "SkyBoxShader.hlsl"
		, "SkyBoxVS"
		, "SkyBoxPS"
		, "SkyBoxGS"
		, ""
		, ""
		, true);

	CreateRootSignature("SkyBoxRootSignature",mShader.rootSignature.Get(), dev);
}

SkyBoxRootSignature::~SkyBoxRootSignature()
{
}
