#include "stdafx.h"
#include "PrimitiveShadowRootSignature.h"
#include "Shader/ShaderCompiler.h"

PrimitiveShadowRootSignature::PrimitiveShadowRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(
		ShaderCompiler::Instance().GetShaderDirPath() + "Primitive3D.hlsl"
		, "PrimitiveShadowVS"
		, "PrimitiveShadowPS"
		, "PrimitiveGS"
		, ""
		, ""
		, true);
	CreateRootSignature("PrimitiveShadowRender", mShader.rootSignature.Get(), dev);
}


PrimitiveShadowRootSignature::~PrimitiveShadowRootSignature()
{
}
