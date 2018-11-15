#pragma once
/**
*	@file Dx12RenderTargetViewDesc.h
*	@brief RenderTargetViewを作成し保持するクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/11
*/

#include "Dx12BufferViewDesc.h"

/**
*	@ingroup Dx12BufferView
*	@class	Dx12RenderTargetViewDesc
*	@brief RenderTargetViewを作成し保持する
*/
class Dx12RenderTargetViewDesc :
	public Dx12BufferViewDesc
{
public:
	/**
	*	@param[in]	format	要素一つに対応するDXGI_FORMAT
	*/
	Dx12RenderTargetViewDesc(DXGI_FORMAT format);
	~Dx12RenderTargetViewDesc();

	/**
	*	@brief	RenderTargetViewをDescriptorHeapの任意の場所に割り当てる
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	cpuHandle	DescriptorHeapの割り当てるCPUHandle
	*	@param[in]	gpuHandle	DescriptorHeapの割り当てるGPUHandle
	*	@param[in]	resource	RenderTargetとして作っているバッファ
	*/
	void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource);
private:
	/**
	*	作成したRenderTargetViewDesc
	*/
	D3D12_RENDER_TARGET_VIEW_DESC mRtvDesc;
};

