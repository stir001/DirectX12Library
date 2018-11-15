#include "stdafx.h"
#include "BillboardPipelineState.h"
#include "Rootsignature/RootSignatureObject.h"

#include "d3dx12.h"

BillboardPipelineState::BillboardPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature, const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{

	D3D12_INPUT_ELEMENT_DESC billboardinputDescs[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "GAMMA",0,DXGI_FORMAT_R32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};

	CD3DX12_RASTERIZER_DESC rastarizer = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	rastarizer.CullMode = D3D12_CULL_MODE_NONE;

	D3D12_BLEND_DESC blendDesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_MAX;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;


	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	gpsDesc.BlendState = blendDesc;
	gpsDesc.DepthStencilState.DepthEnable = true;
	gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.DepthStencilState.StencilEnable = false;
	gpsDesc.InputLayout.NumElements = sizeof(billboardinputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	gpsDesc.InputLayout.pInputElementDescs = billboardinputDescs;
	gpsDesc.pRootSignature = rootsignature->GetRootSignature().Get();
	gpsDesc.RasterizerState = rastarizer;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleMask = 0xffffff;
	gpsDesc.NodeMask = 0;

	SetShaders(gpsDesc, rootsignature->GetShaderDatas());

	CreatePipelineState("BillBoard", gpsDesc, dev);
}


BillboardPipelineState::~BillboardPipelineState()
{
}
