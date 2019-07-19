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

void PipelineStateObject::SetShaders(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const std::shared_ptr<ShaderDatas>& shaders)
{
	if (shaders->GetVS().ptr != nullptr)
	{
		SetShader(gpsDesc.VS, shaders->GetVS());
	}

	if (shaders->GetPS().ptr != nullptr)
	{
		SetShader(gpsDesc.PS , shaders->GetPS());
	}

	if (shaders->GetGS().ptr != nullptr)
	{
		SetShader(gpsDesc.GS, shaders->GetGS());
	}

	if (shaders->GetDS().ptr != nullptr)
	{
		SetShader(gpsDesc.DS, shaders->GetDS());
	}

	if (shaders->GetHS().ptr != nullptr)
	{
		SetShader(gpsDesc.HS, shaders->GetHS());
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
	
	//�u�����h�̐ݒ�
	D3D12_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc;
	rtBlendDesc.BlendEnable = true;
	rtBlendDesc.LogicOpEnable = false;
	rtBlendDesc.SrcBlend = D3D12_BLEND::D3D12_BLEND_ONE;	//�����珑�����ނ���
	rtBlendDesc.DestBlend = D3D12_BLEND::D3D12_BLEND_ZERO;	//�������ݐ�ɂ��łɂ������
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
	
	//�X�g���[���A�E�g�v�b�g�̐ݒ�
	D3D12_STREAM_OUTPUT_DESC streamOutputDesc;
	streamOutputDesc.pSODeclaration = nullptr;
	streamOutputDesc.NumEntries = 0;
	streamOutputDesc.NumStrides = 0;
	streamOutputDesc.pBufferStrides = 0;
	streamOutputDesc.RasterizedStream = 0;

	desc.StreamOutput = streamOutputDesc;

	desc.SampleMask = UINT_MAX;

	//���X�^���C�U�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;	//���X�^���C�Y���ɖ��߂邩��������
	rasterizerDesc.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;	//�ǂ�����J�����O���邩
	rasterizerDesc.FrontCounterClockwise = true;	//�\�����v��肩�ǂ���
	rasterizerDesc.DepthBias = 0;					//�f�v�X�ɑ΂���o�C�A�X�̔䗦(���̒l���f�v�X�ɂ�����̂ŃX�P�[�����Ȃ�����1��-1�ł���?)
	rasterizerDesc.DepthBiasClamp = 0.0f;			//�f�v�X�o�C�A�X�̃N�����v��ۂ̍ő�l
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;		//���z�ɑ΂���X�P�[�����O�̃o�C�A�X
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	desc.RasterizerState = rasterizerDesc;

	//�f�v�X�ƃX�e���V���Ɋւ���ݒ�
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

	desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;		//�g���C�A���O���X�g���b�v���̕s�A������\�����ʂȃC���f�b�N�X�l�̐ݒ�
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	
	DXGI_SAMPLE_DESC sampleDesc;
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	desc.SampleDesc = sampleDesc;
	//�Q�Ƃł���f�o�C�X�̃}�X�N
	desc.NodeMask = 0;
	
	D3D12_CACHED_PIPELINE_STATE cachedPSO;
	cachedPSO.CachedBlobSizeInBytes = 0;
	cachedPSO.pCachedBlob = nullptr;

	desc.CachedPSO = cachedPSO;

	//�f�o�b�O�p�ɒǉ�������������ԂŃR���p�C�������ݒ�Adevice��WARP�̏ꍇ�̂ݗL���A��{��None�ł���
	desc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;

	return desc;
}

void PipelineStateObject::SetShader(D3D12_SHADER_BYTECODE & byteCode, ShaderResource& rsc)
{
	byteCode.pShaderBytecode = rsc.ptr;
	byteCode.BytecodeLength = rsc.size;
}

