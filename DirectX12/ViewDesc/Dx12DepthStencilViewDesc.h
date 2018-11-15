#pragma once
/**
*	@file Dx12DepthStencilViewDesc.h
*	@brief DepthstencilViewを作成する
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/11
*/

#include "Dx12BufferViewDesc.h"

/**
*	@ingroup Dx12BufferView
*	@class Dx12DepthStencilViewDesc
*	@brief DephtStencilViewを作成し保持する
*/
class Dx12DepthStencilViewDesc :
	public Dx12BufferViewDesc
{
public:
	/**
	*	@param[in]	format	要素一つに対応したDXGI_FORMAT
	*/
	Dx12DepthStencilViewDesc(DXGI_FORMAT format);
	~Dx12DepthStencilViewDesc();

	/**
	*	@brief	DepthStencilViewをDescriptorHeapの任意の場所に割り当てる
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	cpuHandle	DescriptorHeapの割り当てるCPUHandle
	*	@param[in]	gpuHandle	DescriptorHeapの割り当てるGPUHandle
	*	@param[in]	resource	DepthStencilBufferとして作っているバッファ
	*/
	void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource);
private:
	D3D12_DEPTH_STENCIL_VIEW_DESC mDsvDesc;
};

