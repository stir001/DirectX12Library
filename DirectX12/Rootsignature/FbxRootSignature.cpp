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

	CreateRootSignature("FbxRootSignature", mShader->rootSignature.Get(), dev);

	mInputElements.resize(3);
	mInputElements[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	mInputElements[1] = { "NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	mInputElements[2] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
}


FbxRootSignature::~FbxRootSignature()
{
}
