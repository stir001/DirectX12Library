#include "stdafx.h"
#include "PipelineStateObject.h"
#include "Util/CharToWChar.h"
#include "d3dx12.h"

PipelineStateObject::PipelineStateObject(const std::string& name, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev):mName(name + "GraphicsPipelineState")
{
	dev->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
	SetName(mName);
}

PipelineStateObject::PipelineStateObject(const std::string & name, D3D12_COMPUTE_PIPELINE_STATE_DESC & cpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev):mName(name + "ComputePipelineState")
{
	dev->CreateComputePipelineState(&cpsDesc, IID_PPV_ARGS(&mPipelineState));
	SetName(mName);
}

PipelineStateObject::PipelineStateObject() :mPipelineState(nullptr), mName()
{
}

void PipelineStateObject::SetPipelineState(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList)
{
	cmdList->SetPipelineState(mPipelineState.Get());
}

PipelineStateObject::~PipelineStateObject()
{
	mPipelineState.Reset();
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> PipelineStateObject::GetPipelineState() const
{
	return mPipelineState;
}

void PipelineStateObject::CreatePipelineState(const std::string& name, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	if (mPipelineState != nullptr) return;
	mName = name + "GraphicsPipelineState";
	HRESULT rsult = dev->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
	SetName(mName);
}

void PipelineStateObject::CreatePipelineState(const std::string & name, D3D12_COMPUTE_PIPELINE_STATE_DESC & gpsDesc,const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	if (mPipelineState != nullptr) return;
	mName = name + "ComputePipelineState";
	HRESULT rsult = dev->CreateComputePipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
	SetName(mName);
}

void PipelineStateObject::SetShaders(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const ShaderDatas& shaders)
{
	if (shaders.vertexShader != nullptr)
	{
		gpsDesc.VS = CD3DX12_SHADER_BYTECODE(shaders.vertexShader.Get());
	}

	if (shaders.pixelShader != nullptr)
	{
		gpsDesc.PS = CD3DX12_SHADER_BYTECODE(shaders.pixelShader.Get());
	}

	if (shaders.geometryShader != nullptr)
	{
		gpsDesc.GS = CD3DX12_SHADER_BYTECODE(shaders.geometryShader.Get());
	}

	if (shaders.domainShader != nullptr)
	{
		gpsDesc.DS = CD3DX12_SHADER_BYTECODE(shaders.domainShader.Get());
	}

	if (shaders.hullShader != nullptr)
	{
		gpsDesc.HS = CD3DX12_SHADER_BYTECODE(shaders.hullShader.Get());
	}
}

void PipelineStateObject::SetName(const std::string & name)
{
	std::wstring buf;
	ToWChar(buf, name);
	mPipelineState->SetName(buf.data());
}

