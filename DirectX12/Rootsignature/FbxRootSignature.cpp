#include "stdafx.h"
#include "FbxRootSignature.h"
#include "Shader/ShaderCompiler.h"

const std::string FBX_SHADER_PATH = ShaderCompiler::Instance().GetShaderDirPath() + "FbxShader.hlsl";
const std::string FBX_VERTEXSHADER_NAME = "FbxVS";
const std::string FBX_PIXCELSHADER_NAME = "FbxPS";
const std::string FBX_GEOMETRYSHADER_NAME = "FbxGS";
const std::string FBX_HULLSHADER_NAME = "";
const std::string FBX_DOMAINSHADER_NAME = "";


FbxRootSignature::FbxRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(
		FBX_SHADER_PATH,
		FBX_VERTEXSHADER_NAME,
		FBX_PIXCELSHADER_NAME,
		FBX_GEOMETRYSHADER_NAME,
		FBX_HULLSHADER_NAME,
		FBX_DOMAINSHADER_NAME,
		true);

	CreateRootSignature("FbxRootSignature", mShader.rootSignature.Get(), dev);
}


FbxRootSignature::~FbxRootSignature()
{
}
