#include "stdafx.h"
#include "PipelineStateObject.h"
#include "Rootsignature/RootSignatureObject.h"
#include "Util/CharToWChar.h"

PipelineStateObject::PipelineStateObject(const std::string& name
	, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
	: mName(name + "GraphicsPipelineState")
{
	dev->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
	SetName(mName);
}

PipelineStateObject::PipelineStateObject(const std::string & name
	, D3D12_COMPUTE_PIPELINE_STATE_DESC& cpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
	: mName(name + "ComputePipelineState")
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

void PipelineStateObject::CreatePipelineState(const std::string& name
	, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	if (mPipelineState != nullptr) return;
	mName = name + "GraphicsPipelineState";
	HRESULT rsult = dev->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
	SetName(mName);
}

void PipelineStateObject::CreatePipelineState(const std::string& name
	, D3D12_COMPUTE_PIPELINE_STATE_DESC& gpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	if (mPipelineState != nullptr) return;
	mName = name + "ComputePipelineState";
	HRESULT rsult = dev->CreateComputePipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
	SetName(mName);
}

void PipelineStateObject::SetInputElement(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const std::shared_ptr<RootSignatureObject>& rootSignature)
{
	gpsDesc.InputLayout.pInputElementDescs = rootSignature->GetInputElementDesc().data();
	gpsDesc.InputLayout.NumElements = static_cast<unsigned int>(rootSignature->GetInputElementDesc().size());
}

void PipelineStateObject::SetShaders(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const ShaderDatas& shaders)
{
	if (shaders.vertexShader != nullptr)
	{
		SetShader(gpsDesc.VS, shaders.vertexShader.Get());
	}

	if (shaders.pixelShader != nullptr)
	{
		SetShader(gpsDesc.PS , shaders.pixelShader.Get());
	}

	if (shaders.geometryShader != nullptr)
	{
		SetShader(gpsDesc.GS, shaders.geometryShader.Get());
	}

	if (shaders.domainShader != nullptr)
	{
		SetShader(gpsDesc.DS, shaders.domainShader.Get());
	}

	if (shaders.hullShader != nullptr)
	{
		SetShader(gpsDesc.HS, shaders.hullShader.Get());
	}
}

void PipelineStateObject::SetRootSignatureConfigure(D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, const std::shared_ptr<RootSignatureObject>& rootSignature)
{
	gpsDesc.pRootSignature = rootSignature->GetRootSignature().Get();
	SetInputElement(gpsDesc, rootSignature);
	SetShaders(gpsDesc, rootSignature->GetShaderDatas());
}

void PipelineStateObject::SetName(const std::string & name)
{
	std::wstring buf;
	ToWChar(buf, name);
	mPipelineState->SetName(buf.data());
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateObject::GetDefalutPipelineStateDesc() const
{
	//GraphicsPSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	
	//ブレンドの設定
	D3D12_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc;
	rtBlendDesc.BlendEnable = true;
	rtBlendDesc.LogicOpEnable = false;
	rtBlendDesc.SrcBlend = D3D12_BLEND::D3D12_BLEND_ONE;	//今から書き込むもの
	rtBlendDesc.DestBlend = D3D12_BLEND::D3D12_BLEND_ZERO;	//書き込み先にすでにあるもの
	rtBlendDesc.BlendOp = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	rtBlendDesc.SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
	rtBlendDesc.DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
	rtBlendDesc.BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_MAX;
	rtBlendDesc.LogicOp = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
	rtBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	for (unsigned int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
	{
		blendDesc.RenderTarget[i] = rtBlendDesc;
	}

	desc.BlendState = blendDesc;
	
	//ストリームアウトプットの設定
	D3D12_STREAM_OUTPUT_DESC streamOutputDesc;
	streamOutputDesc.pSODeclaration = nullptr;
	streamOutputDesc.NumEntries = 0;
	streamOutputDesc.NumStrides = 0;
	streamOutputDesc.pBufferStrides = 0;
	streamOutputDesc.RasterizedStream = 0;

	desc.StreamOutput = streamOutputDesc;

	desc.SampleMask = UINT_MAX;

	//ラスタライザの設定
	D3D12_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;	//ラスタライズ時に埋めるか線だけか
	rasterizerDesc.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;	//どちらをカリングするか
	rasterizerDesc.FrontCounterClockwise = true;	//表が時計回りかどうか
	rasterizerDesc.DepthBias = 0;					//デプスに対するバイアスの比率(この値をデプスにかけるのでスケールしない限り1か-1でいい?)
	rasterizerDesc.DepthBiasClamp = 0.0f;			//デプスバイアスのクランプる際の最大値
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;		//勾配に対するスケーリングのバイアス
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	desc.RasterizerState = rasterizerDesc;

	//デプスとステンシルに関する設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS;
	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = 0;
	depthStencilDesc.StencilWriteMask = 0;
	depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;
	depthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;

	desc.DepthStencilState = depthStencilDesc;

	desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;		//トライアングルストリップ時の不連続性を表す特別なインデックス値の設定
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	
	DXGI_SAMPLE_DESC sampleDesc;
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	desc.SampleDesc = sampleDesc;
	//参照できるデバイスのマスク
	desc.NodeMask = 0;
	
	D3D12_CACHED_PIPELINE_STATE cachedPSO;
	cachedPSO.CachedBlobSizeInBytes = 0;
	cachedPSO.pCachedBlob = nullptr;

	desc.CachedPSO = cachedPSO;

	//デバッグ用に追加情報を持った状態でコンパイルされる設定、deviceがWARPの場合のみ有効、基本はNoneでいい
	desc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;

	return desc;
}

void PipelineStateObject::SetShader(D3D12_SHADER_BYTECODE & byteCode, ID3DBlob * blob)
{
	byteCode.pShaderBytecode = blob->GetBufferPointer();
	byteCode.BytecodeLength = blob->GetBufferSize();
}

