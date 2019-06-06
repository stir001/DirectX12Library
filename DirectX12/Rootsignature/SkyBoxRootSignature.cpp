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

	mInputElements.resize(2);
	mInputElements[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	mInputElements[1] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
}

SkyBoxRootSignature::~SkyBoxRootSignature()
{
}
