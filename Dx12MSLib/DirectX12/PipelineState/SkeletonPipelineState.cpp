#include "stdafx.h"
#include "SkeletonPipelineState.h"
#include "Rootsignature/RootSignatureObject.h"
#include "d3dx12.h"

SkeletonPipelineState::SkeletonPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature, const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	CD3DX12_RASTERIZER_DESC rastarizer = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	rastarizer.CullMode = D3D12_CULL_MODE_NONE;

	D3D12_INPUT_ELEMENT_DESC sklinputDescs[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	gpsDesc.DepthStencilState.DepthEnable = false;
	gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpsDesc.DepthStencilState.StencilEnable = false;
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.InputLayout.NumElements = sizeof(sklinputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	gpsDesc.InputLayout.pInputElementDescs = sklinputDescs;
	gpsDesc.pRootSignature = rootsignature->GetRootSignature().Get();
	gpsDesc.RasterizerState = rastarizer;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleMask = 0xffffff;
	gpsDesc.NodeMask = 0;

	auto& shaderData = rootsignature->GetShaderDatas();

	SetShaders(gpsDesc, shaderData);

	CreatePipelineState("Skeleton", gpsDesc, dev);
}


SkeletonPipelineState::~SkeletonPipelineState()
{
}
