#pragma once
/**
*	@file Dx12ShaderResourceViewDesc.h
*	@brief ShadreResourceViewを作成し保持するクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/11
*/

#include "Dx12BufferViewDesc.h"

/**
*	@ingroup Dx12BufferView
*	@class	Dx12ShaderResourceViewDesc
*	@brief ShadreResourceViewを作成し保持する
*/
class Dx12ShaderResourceViewDesc :
	public Dx12BufferViewDesc
{
public:
	/**
	*	@param[in]	format	要素一つに対応するDXGI_FORMAT
	*/
	Dx12ShaderResourceViewDesc(DXGI_FORMAT format);
	~Dx12ShaderResourceViewDesc();

	/**
	*	@brief	ShadreResourceViewをDescriptorHeapの任意の場所に割り当てる
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	cpuHandle	DescriptorHeapの割り当てるCPUHandle
	*	@param[in]	gpuHandle	DescriptorHeapの割り当てるGPUHandle
	*	@param[in]	resource	ShaderResourceとして作っているバッファ
	*/
	void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr);
private:
	/**
	*	作成したShaderResourceView
	*/
	D3D12_SHADER_RESOURCE_VIEW_DESC mSrvDesc;
};

