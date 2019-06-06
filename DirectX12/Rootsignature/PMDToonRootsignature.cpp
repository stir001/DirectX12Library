#include "stdafx.h"
#include "PMDToonRootsignature.h"
#include "Shader/ShaderCompiler.h"


PMDToonRootsignature::PMDToonRootsignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	ShaderCompiler::Instance().AddDefineMacro("CAMERA_REGISTER", "b0");
	ShaderCompiler::Instance().AddDefineMacro("LIGHT_REGISTER", "b1");

	mShader = ShaderCompiler::Instance().CompileShader(ShaderCompiler::Instance().GetShaderDirPath() + "PMDToonShader.hlsl"
		, "PmdToonVS"
		, "PmdToonPS"
		, "PmdGS"
		, ""
		, ""
		, true);
	CreateRootSignature("PmdToon", mShader.rootSignature.Get(), dev);

	mInputElements.resize(5);
	mInputElements[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	mInputElements[0] = { "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	mInputElements[0] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	mInputElements[0] = { "BONENO",0,DXGI_FORMAT_R16G16_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	mInputElements[0] = { "WEIGHT",0,DXGI_FORMAT_R8_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
}


PMDToonRootsignature::~PMDToonRootsignature()
{
}
