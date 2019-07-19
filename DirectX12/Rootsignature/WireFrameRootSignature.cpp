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

	CreateRootSignature("WireFrameRootSignature", mShader->rootSignature.Get(), dev);

	mInputElements.resize(2);
	mInputElements[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	mInputElements[1] = { "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
}


WireFrameRootSignature::~WireFrameRootSignature()
{
}
