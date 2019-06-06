#include "stdafx.h"
#include "PrimitiveNormalMapRootSignature.h"
#include "Shader/ShaderCompiler.h"

PrimitiveNormalMapRootSignature::PrimitiveNormalMapRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(
		ShaderCompiler::Instance().GetShaderDirPath() + "NormalmapShader.hlsl",
		"NormalMapVS",
		"NormalMapPS",
		"NormalMapGS",
		"",
		"",
		true
	);

	CreateRootSignature("NormalMapRootSignature",mShader.rootSignature.Get(), dev);

	mInputElements.resize(9);
	mInputElements[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0 ,D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	mInputElements[0] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	mInputElements[0] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	mInputElements[0] = { "INSTANCEMAT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 1, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	mInputElements[0] = { "INSTANCEMAT", 1, DXGI_FORMAT_R32G32B32A32_FLOAT , 1, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	mInputElements[0] = { "INSTANCEMAT", 2, DXGI_FORMAT_R32G32B32A32_FLOAT , 1, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	mInputElements[0] = { "INSTANCEMAT", 3, DXGI_FORMAT_R32G32B32A32_FLOAT , 1, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	mInputElements[0] = { "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 1, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	mInputElements[0] = { "INSTANCECOLOR", 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1 , D3D12_APPEND_ALIGNED_ELEMENT , D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
}

PrimitiveNormalMapRootSignature::~PrimitiveNormalMapRootSignature()
{
}
