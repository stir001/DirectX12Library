#pragma once
/**
*	@file Dx12UnorderedAccessViewDesc.h
*	@brief UnorderedAccessViewを作成し保持するクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/11
*/

#include "Dx12BufferViewDesc.h"

/**
*	@ingroup Dx12BufferView
*	@class	Dx12UnorderedAccessViewDesc
*	@brief UnorderdAccessViewを作成し保持する
*/
class Dx12UnorderedAccessViewDesc :
	public Dx12BufferViewDesc
{
public:
	/**
	*	@param[in]	numElements	要素の総数
	*	@param[in]	stride		一要素のサイズ
	*/
	Dx12UnorderedAccessViewDesc(UINT numElements, UINT stride);
	~Dx12UnorderedAccessViewDesc();

	/**
	*	@brief	UnorderedAccessViewをDescriptorHeapの任意の場所に割り当てる
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	cpuHandle	DescriptorHeapの割り当てるCPUHandle
	*	@param[in]	gpuHandle	DescriptorHeapの割り当てるGPUHandle
	*	@param[in]	resource	UnorderdAccessとして作っているバッファ
	*/
	void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr);
private:
	/**
	*	作成したUnorderdAccessView
	*/
	const D3D12_UNORDERED_ACCESS_VIEW_DESC mUavDesc;
};

