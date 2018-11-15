#pragma once
#include <d2d1_3.h>
#include <d3d11on12.h>
#include <wrl.h>
#include <dwrite.h>
#include <string>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dwrite.lib")

class Dx12TextDevice
{
public:
	Dx12TextDevice(Microsoft::WRL::ComPtr<ID3D12Device> d3d12Device, const Microsoft::WRL::ComPtr<ID3D12CommandQueue> d3d12CommandQueue);
	~Dx12TextDevice();

	void Render(std::string& str);
private:
	Microsoft::WRL::ComPtr<ID2D1Device2> mD2D1Device;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext2> mD2DeviceContext;
	Microsoft::WRL::ComPtr<IDWriteFactory> mDWriteFactory;
	Microsoft::WRL::ComPtr<ID2D1Factory3> mD2DFactory;
	Microsoft::WRL::ComPtr<ID3D11On12Device> mD11onD12Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mD3D11DeviceContext;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mTextBrush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> mTextFormat;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommnadList;
};

