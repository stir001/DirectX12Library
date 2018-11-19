#include "stdafx.h"
#include "BillboardRootSignature.h"
#include "Shader/ShaderCompiler.h"

BillboardRootSignature::BillboardRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(
		ShaderCompiler::Instance().GetShaderDirPath() + "Billboard.hlsl",
		"BillboardVS",
		"BillboardPS",
		"BillboardGS",
		"",
		"",
		true
	);

	CreateRootSignature("BillboardRootSignature", mShader.rootSignature.Get(), dev);
}


BillboardRootSignature::~BillboardRootSignature()
{
}
