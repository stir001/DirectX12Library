#include "stdafx.h"
#include "Image3DRootSignature.h"
#include "Shader/ShaderCompiler.h"

Image3DRootSignature::Image3DRootSignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(
		ShaderCompiler::Instance().GetShaderDirPath() + "Image3DShader.hlsl",
		"Image3DVS",
		"Image3DPS",
		"",
		"",
		"",
		true
	);

	CreateRootSignature("Image3DRootSignature", mShader.rootSignature.Get(), dev);
}


Image3DRootSignature::~Image3DRootSignature()
{
}
