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

	mInputElements.resize(3);
	mInputElements[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	mInputElements[1] = { "NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	mInputElements[2] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
}


FbxNonShadeRootSignature::~FbxNonShadeRootSignature()
{
}
