#include "stdafx.h"
#include "PrimitiveRootSignature.h"
#include "Shader/ShaderCompiler.h"

PrimitiveRootSignature::PrimitiveRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(ShaderCompiler::Instance().GetShaderDirPath() + "Primitive3D.hlsl",
		"PrimitiveVS",
		"PrimitivePS", 
		"PrimitiveGS", "", "", true);
	CreateRootSignature("PrimitiveRootSignature",mShader.rootSignature.Get(), dev);
}

PrimitiveRootSignature::~PrimitiveRootSignature()
{
}
