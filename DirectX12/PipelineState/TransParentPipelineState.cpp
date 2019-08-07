#include "stdafx.h"
#include "TransParentPipelineState.h"

TransParentPipelineState::TransParentPipelineState(const std::string& name,
	const std::shared_ptr<RootSignatureObject>& rootsignature,
	const Microsoft::WRL::ComPtr<ID3D12Device>& dev, bool isUseDepth)
{
	auto gps = GetDefalutPipelineStateDesc();
	gps.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;

	auto rtDesc = gps.BlendState.RenderTarget[0];
	rtDesc.BlendEnable = true;
	rtDesc.BlendOp = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	rtDesc.DestBlend = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	rtDesc.SrcBlend = D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
	rtDesc.BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_MAX;
	rtDesc.DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_ZERO;
	rtDesc.SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
	rtDesc.LogicOpEnable = false;
	rtDesc.LogicOp = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
	rtDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL;

	if (!isUseDepth)
	{
		gps.DepthStencilState.DepthEnable = false;
	}

	gps.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
	
	gps.BlendState.RenderTarget[0] = rtDesc;

	SetRootSignatureConfigure(gps, rootsignature);

	CreatePipelineState(name, gps, dev);
}

TransParentPipelineState::~TransParentPipelineState()
{
}
