#include "stdafx.h"
#include "PrimitiveShadowRenderRootSignature.h"
#include "Shader/ShaderCompiler.h"

PrimitiveShadowRenderRootSignature::PrimitiveShadowRenderRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(
		ShaderCompiler::Instance().GetShaderDirPath() + "Primitive3D.hlsl"
		, "ShadowVS"
		, ""
		, ""
		, ""
		, ""
		, true);

	CreateRootSignature("PrimitiveShadowmap", mShader.rootSignature.Get(), dev);
}


PrimitiveShadowRenderRootSignature::~PrimitiveShadowRenderRootSignature()
{
}
