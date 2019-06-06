#include "stdafx.h"
#include "Image3DRootSignature.h"
#include "Shader/ShaderCompiler.h"

Image3DRootSignature::Image3DRootSignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(
		ShaderCompiler::Instance().GetShaderDirPath() + "Image3DShader.hlsl",
		"Image3DVS",
		"Image3DPS",
		"Image3DGS",
		"",
		"",
		true);

	CreateRootSignature("Image3DRootSignature", mShader.rootSignature.Get(), dev);

	mInputElements.resize(4);
	mInputElements[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	mInputElements[1] = { "NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	mInputElements[2] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	mInputElements[3] = { "GAMMA",0,DXGI_FORMAT_R32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
}


Image3DRootSignature::~Image3DRootSignature()
{
}
