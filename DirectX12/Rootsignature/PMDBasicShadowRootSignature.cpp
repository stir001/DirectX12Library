#include "stdafx.h"
#include "PMDBasicShadowRootSignature.h"
#include "Shader/ShaderCompiler.h"

PMDBasicShadowRootSignature::PMDBasicShadowRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	ShaderCompiler::Instance().AddDefineMacro("CAMERA_REGISTER", "b0");
	ShaderCompiler::Instance().AddDefineMacro("LIGHT_REGISTER", "b1");

	mShader = ShaderCompiler::Instance().CompileShader(ShaderCompiler::Instance().GetShaderDirPath() + "PMDBasicShader.hlsl"
		, "BasicShadowVS"
		, "BasicShadowPS"
		, "PmdGS"
		, ""
		, ""
		, true);

	CreateRootSignature("PMDBasicShadow", mShader.rootSignature.Get(), dev);
}


PMDBasicShadowRootSignature::~PMDBasicShadowRootSignature()
{
}
