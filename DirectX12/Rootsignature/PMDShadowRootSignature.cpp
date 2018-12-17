#include "stdafx.h"
#include "PMDShadowRootSignature.h"
#include "Shader/ShaderCompiler.h"

PMDShadowRootSignature::PMDShadowRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(ShaderCompiler::Instance().GetShaderDirPath() + "PMDToonShader.hlsl"
		, "PmdShadowVS"
		, ""
		, ""
		, ""
		, ""
		, true);
}


PMDShadowRootSignature::~PMDShadowRootSignature()
{
}
