#include "stdafx.h"
#include "PMDShadowPipelineState.h"
#include "Rootsignature/RootSignatureObject.h"

PMDShadowPipelineState::PMDShadowPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature,
	const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	D3D12_INPUT_ELEMENT_DESC inputDescs[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "BONENO",0,DXGI_FORMAT_R16G16_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "WEIGHT",0,DXGI_FORMAT_R8_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
	};

	D3D12_RASTERIZER_DESC rastarizer;
	rastarizer.FillMode = D3D12_FILL_MODE_SOLID;
	rastarizer.CullMode = D3D12_CULL_MODE_NONE;
	rastarizer.FrontCounterClockwise = false;
	rastarizer.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rastarizer.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rastarizer.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rastarizer.DepthClipEnable = true;
	rastarizer.MultisampleEnable = false;
	rastarizer.AntialiasedLineEnable = false;
	rastarizer.ForcedSampleCount = 0;
	rastarizer.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	D3D12_RENDER_TARGET_BLEND_DESC rtDesc;
	rtDesc.BlendEnable = false;
	rtDesc.LogicOpEnable = false;
	rtDesc.SrcBlend = D3D12_BLEND_ONE;
	rtDesc.DestBlend = D3D12_BLEND_ZERO;
	rtDesc.BlendOp = D3D12_BLEND_OP_ADD;
	rtDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	rtDesc.DestBlendAlpha = D3D12_BLEND_ONE;
	rtDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	rtDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
	rtDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	for (unsigned int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
	{
		blendDesc.RenderTarget[i] = rtDesc;
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	gpsDesc.BlendState = blendDesc;
	gpsDesc.DepthStencilState.DepthEnable = true;
	gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.DepthStencilState.StencilEnable = false;
	gpsDesc.InputLayout.NumElements = sizeof(inputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	gpsDesc.InputLayout.pInputElementDescs = inputDescs;
	gpsDesc.pRootSignature = rootsignature->GetRootSignature().Get();
	gpsDesc.RasterizerState = rastarizer;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleMask = 0xffffff;
	gpsDesc.NodeMask = 0;

	gpsDesc.pRootSignature = rootsignature->GetRootSignature().Get();
	SetShaders(gpsDesc, rootsignature->GetShaderDatas());

	CreatePipelineState("PMDBasic", gpsDesc, dev);
}


PMDShadowPipelineState::~PMDShadowPipelineState()
{
}
