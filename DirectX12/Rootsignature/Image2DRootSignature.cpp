#include "stdafx.h"
#include "Image2DRootSignature.h"
#include "Shader/ShaderCompiler.h"

Image2DRootSignature::Image2DRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(
		ShaderCompiler::Instance().GetShaderDirPath() + "ImageShader.hlsl",
		"ImageVS",
		"ImagePS",
		"",
		"",
		"",
		true
	);

	CreateRootSignature("Image2DRootSignature", mShader.rootSignature.Get(), dev);
}


Image2DRootSignature::~Image2DRootSignature()
{
}
