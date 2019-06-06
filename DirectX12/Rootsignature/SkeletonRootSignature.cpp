#include "stdafx.h"
#include "SkeletonRootSignature.h"
#include "Shader/ShaderCompiler.h"

SkeletonRootSignature::SkeletonRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(ShaderCompiler::Instance().GetShaderDirPath() + "SkeletonShader.hlsl",
		"SklVS",
		"SklPS",
		"",
		"",
		"",
		true);
	CreateRootSignature("SkeletonRootSignature", mShader.rootSignature.Get(), dev);

	mInputElements.resize(1);
	mInputElements[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
}


SkeletonRootSignature::~SkeletonRootSignature()
{
}
