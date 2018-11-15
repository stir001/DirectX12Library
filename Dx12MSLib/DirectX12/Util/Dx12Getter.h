#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>

namespace dx12_getter
{
	struct ResourceDescSet
	{
		//https://msdn.microsoft.com/ja-jp/library/windows/desktop/dn903813(v=vs.85).aspx
		D3D12_RESOURCE_DESC rscDesc;

		//https://msdn.microsoft.com/ja-jp/library/windows/desktop/dn770373(v=vs.85).aspx
		D3D12_HEAP_PROPERTIES heapProp;
	};

	D3D12_RESOURCE_DESC GetResourceDesc(unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc(unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);
	ResourceDescSet GetConstantBufferDescSet(unsigned int elementsize, unsigned int elementcount);

	unsigned int GetDxgiFormatByteSize(DXGI_FORMAT format);
}