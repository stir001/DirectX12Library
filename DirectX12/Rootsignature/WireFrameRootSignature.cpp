#include "stdafx.h"
#include "WireFrameRootSignature.h"
#include "Shader/ShaderCompiler.h"

WireFrameRootSignature::WireFrameRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(
		ShaderCompiler::Instance().GetShaderDirPath() + "WireFrameShader.hlsl"
		, "WireVS"
		, "WirePS"
		, "WireGS"
		, ""
		, ""
		, true
	);

	CreateRootSignature("WireFrameRootSignature", mShader.rootSignature.Get(), dev);
}


WireFrameRootSignature::~WireFrameRootSignature()
{
}
