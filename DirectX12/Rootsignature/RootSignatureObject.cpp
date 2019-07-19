#include "stdafx.h"
#include "RootSignatureObject.h"
#include "Util/CharToWChar.h"
#include <d3d12.h>

RootSignatureObject::RootSignatureObject(): mRootSignature(nullptr)
{
}

RootSignatureObject::RootSignatureObject(const std::string& name, ID3D10Blob* signatureBlob,Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	CreateRootSignature(name, signatureBlob, dev);
}

RootSignatureObject::~RootSignatureObject()
{
	mRootSignature.Reset();
}

void RootSignatureObject::SetShaderData(const std::shared_ptr<ShaderDatas>& shader)
{
	mShader = shader;
}

const std::shared_ptr<ShaderDatas>& RootSignatureObject::GetShaderDatas() const
{
	return mShader;
}

const Microsoft::WRL::ComPtr<ID3D12RootSignature>& RootSignatureObject::GetRootSignature() const
{
	return mRootSignature;
}

const std::vector<D3D12_INPUT_ELEMENT_DESC>& RootSignatureObject::GetInputElementDesc() const
{
	return mInputElements;
}

void RootSignatureObject::CreateRootSignature(const std::string& name, ID3DBlob * signatureBlob, const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	dev->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
	std::wstring str;
	ToWChar(str, name);
	mRootSignature->SetName(str.data());
}

