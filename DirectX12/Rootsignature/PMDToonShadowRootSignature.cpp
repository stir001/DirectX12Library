#include "stdafx.h"
#include "PMDToonShadowRootSignature.h"
#include "Shader/ShaderCompiler.h"

PMDToonShadowRootSignature::PMDToonShadowRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	ShaderCompiler::Instance().AddDefineMacro("CAMERA_REGISTER", "b0");
	ShaderCompiler::Instance().AddDefineMacro("LIGHT_REGISTER", "b1");

	mShader = ShaderCompiler::Instance().CompileShader(ShaderCompiler::Instance().GetShaderDirPath() + "PMDToonShader.hlsl"
		, "PmdToonShadowVS"
		, "PmdToonShadowPS"
		, "PmdGS"
		, ""
		, ""
		, true);
	CreateRootSignature("PmdToonShadow", mShader.rootSignature.Get(), dev);
}


PMDToonShadowRootSignature::~PMDToonShadowRootSignature()
{
}
