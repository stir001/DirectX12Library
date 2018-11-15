#include "stdafx.h"
#include "Dx12TextDevice.h"
#include <d3d12.h>
#include <cassert>
#include <iostream>

Dx12TextDevice::Dx12TextDevice(Microsoft::WRL::ComPtr<ID3D12Device> d3d12Device, const Microsoft::WRL::ComPtr<ID3D12CommandQueue> d3d12CommandQueue)
{
	unsigned int d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device;
	D3D11On12CreateDevice(
		d3d12Device.Get(),
		d3d11DeviceFlags,
		nullptr,
		0,
		(IUnknown**)d3d12CommandQueue.GetAddressOf(),
		1,
		0,
		d3d11Device.GetAddressOf(),
		mD3D11DeviceContext.GetAddressOf(),
		nullptr
	);

	assert(SUCCEEDED(d3d11Device.As(&mD11onD12Device)));

	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&mD2DFactory));
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	mD11onD12Device.As(&dxgiDevice);
	mD2DFactory->CreateDevice(dxgiDevice.Get(), mD2D1Device.GetAddressOf());
	D2D1_DEVICE_CONTEXT_OPTIONS deviceOptions = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
	mD2D1Device->CreateDeviceContext(deviceOptions, mD2DeviceContext.GetAddressOf());
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(mDWriteFactory), (IUnknown**)mDWriteFactory.GetAddressOf());

	mD2DeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), mTextBrush.GetAddressOf());
	mDWriteFactory->CreateTextFormat(
		L"ƒƒCƒŠƒI",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		50,
		L"en-us",
		mTextFormat.GetAddressOf()
	);
	mTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	mTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

Dx12TextDevice::~Dx12TextDevice()
{
}

void Dx12TextDevice::Render(std::string & str)
{
}
