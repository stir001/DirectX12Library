#include "stdafx.h"
#include "PMDShadowRootSignature.h"
#include "Shader/ShaderCompiler.h"

PMDShadowRootSignature::PMDShadowRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	ShaderCompiler::Instance().AddDefineMacro("CAMERA_REGISTER", "b0");
	ShaderCompiler::Instance().AddDefineMacro("LIGHT_REGISTER", "b1");

	mShader = ShaderCompiler::Instance().CompileShader(ShaderCompiler::Instance().GetShaderDirPath() + "PMDToonShader.hlsl"
		, "PmdShadowVS"
		, ""
		, ""
		, ""
		, ""
		, true);

	CreateRootSignature("PMDShadow", mShader.rootSignature.Get(), dev);
}


PMDShadowRootSignature::~PMDShadowRootSignature()
{
}
