#include "stdafx.h"
#include "PMDToonRootsignature.h"
#include "Shader/ShaderCompiler.h"


PMDToonRootsignature::PMDToonRootsignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(ShaderCompiler::Instance().GetShaderDirPath() + "PMDToonShader.hlsl"
		, "PmdToonVS"
		, "PmdToonPS"
		, "PmdGS"
		, ""
		, ""
		, true);
	CreateRootSignature("PmdToon", mShader.rootSignature.Get(), dev);
}


PMDToonRootsignature::~PMDToonRootsignature()
{
}
