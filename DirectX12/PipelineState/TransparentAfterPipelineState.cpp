#include "stdafx.h"
#include "TransparentAfterPipelineState.h"


TransparentAfterPipelineState::TransparentAfterPipelineState(const std::string& name,
	const std::shared_ptr<RootSignatureObject>& rootsignature,
	const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	auto gps = GetDefalutPipelineStateDesc();
	gps.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;

	auto rtDesc = gps.BlendState.RenderTarget[0];
	rtDesc.BlendEnable = true;
	rtDesc.BlendOp = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	rtDesc.DestBlend = D3D12_BLEND::D3D12_BLEND_DEST_ALPHA;
	rtDesc.SrcBlend = D3D12_BLEND::D3D12_BLEND_INV_DEST_ALPHA;
	rtDesc.BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_MAX;
	rtDesc.DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
	rtDesc.SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
	rtDesc.LogicOpEnable = false;
	rtDesc.LogicOp = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
	rtDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL;

	gps.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_GREATER;
	gps.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;

	gps.BlendState.RenderTarget[0] = rtDesc;

	SetRootSignatureConfigure(gps, rootsignature);

	CreatePipelineState(name, gps, dev);
}


TransparentAfterPipelineState::~TransparentAfterPipelineState()
{
}
