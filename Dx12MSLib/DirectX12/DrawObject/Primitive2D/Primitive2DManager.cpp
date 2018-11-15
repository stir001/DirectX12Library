#include "stdafx.h"
#include "Primitive2DManager.h"
#include "Primitive2DLine.h"
#include "RenderingPath/Manager/RenderingPathManager.h"
#include "Master/Dx12Ctrl.h"
#include "Rootsignature/RootSignatureObject.h"
#include "Shader/ShaderCompiler.h"
#include "PipelineState/PipelineStateObject.h"
#include "d3dx12.h"

Primitive2DManager* Primitive2DManager::mInstance = nullptr;

Primitive2DManager::Primitive2DManager()
{
	mCmdList = RenderingPathManager::Instance().GetRenderingPathCommandList(0);
	CreateRootsignature(Dx12Ctrl::Instance().GetDev());
	CreatePipelineState(Dx12Ctrl::Instance().GetDev());
}


Primitive2DManager::~Primitive2DManager()
{
}

std::shared_ptr<Primitive2DLine> Primitive2DManager::CreatePrimitive2DLine(const DirectX::XMFLOAT3 & point1, const DirectX::XMFLOAT3 & point2)
{
	std::shared_ptr<Primitive2DLine> line = std::make_shared<Primitive2DLine>(point1, point2, Dx12Ctrl::Instance().GetDev(), mCmdList);
	line->SetPipelineState(mPipelinestate);
	line->SetRootSignature(mRootsignature);
	return line;
}

void Primitive2DManager::CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	D3D12_INPUT_ELEMENT_DESC pmv2DinputDescs[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};

	CD3DX12_RASTERIZER_DESC rastarizer = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	rastarizer.CullMode = D3D12_CULL_MODE_NONE;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);	//ブレンドするか
	gpsDesc.DepthStencilState.DepthEnable = false;
	gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.DepthStencilState.StencilEnable = false;
	gpsDesc.InputLayout.NumElements = sizeof(pmv2DinputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	gpsDesc.InputLayout.pInputElementDescs = pmv2DinputDescs;	//要素へのポインタ(先頭?)
	gpsDesc.pRootSignature = mRootsignature->GetRootSignature().Get();
	gpsDesc.RasterizerState = rastarizer;	//ラスタライザーの設定
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleMask = 0xffffff;
	gpsDesc.NodeMask = 0;

	gpsDesc.VS = CD3DX12_SHADER_BYTECODE(mShader.vertexShader.Get());
	gpsDesc.PS = CD3DX12_SHADER_BYTECODE(mShader.pixelShader.Get());

	mPipelinestate = std::make_shared<PipelineStateObject>("Primitive2D", gpsDesc, dev);
}

void Primitive2DManager::CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(
		ShaderCompiler::Instance().GetShaderDirPath() + "Primitive2D.hlsl",
		"Primitive2DVS",
		"Primitive2DPS",
		"",
		"",
		"",
		true
	);

	mRootsignature = (std::make_shared<RootSignatureObject>("Primitive2DRootSignature",mShader.rootSignature.Get(), dev));
}
