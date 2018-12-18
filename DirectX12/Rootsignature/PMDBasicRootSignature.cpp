#include "stdafx.h"
#include "PMDBasicRootSignature.h"
#include "Shader/ShaderCompiler.h"

PMDBasicRootSignature::PMDBasicRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	ShaderCompiler::Instance().AddDefineMacro("CAMERA_REGISTER", "b0");
	ShaderCompiler::Instance().AddDefineMacro("LIGHT_REGISTER", "b1");

	mShader = ShaderCompiler::Instance().CompileShader(ShaderCompiler::Instance().GetShaderDirPath() + "PMDBasicShader.hlsl"
		, "BasicVS"
		, "BasicPS"
		, "PmdGS"
		, ""
		, ""
		, true);

	CreateRootSignature("PMDBasic", mShader.rootSignature.Get(),dev);
}


PMDBasicRootSignature::~PMDBasicRootSignature()
{
}
