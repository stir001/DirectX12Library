#pragma once
/**
*	@file Dx12ConstantBufferViewDesc.h
*	@brief ConstantBufferVIewDescを作成するクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/11
*/
#include "Dx12BufferViewDesc.h"

/**
*	@ingroup Dx12BufferView
*	@class Dx12ConstantBufferViewDesc
*	@brief D3D12_CONSTANT_BUFFER_VIEW_DESCを必要個数作成し保持する
*/
class Dx12ConstantBufferViewDesc :
	public Dx12BufferViewDesc
{
public:
	/**
	*	@param[in]	gpuVirtualAddress	BufferのgpuVirtualAddress
	*	@param[in]	sizeInBytes		Bufferのbyte単位のサイズ
	*	@param[in]	elementCount	要素の総数
	*/
	Dx12ConstantBufferViewDesc(D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress, UINT sizeInBytes, unsigned int elementCount);
	~Dx12ConstantBufferViewDesc();

	/**
	*	@brief	ConstantBufferViewDescをDescriptorHeapの任意の部分に設定する
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	cpuHandle	設定するDescriptorHeapのCPUHandle
	*	@param[in]	gpuHandle	設定するDescriptorHeapのGPUhandle
	*	@param[in]	resource	コンスタントバッファとして作ったID3D12Resource;
	*/
	void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource);

	/**
	*	@brief	同じConstantBufferViewをいくつ持っているかを返す
	*	@return		何個のViewを持っているか
	*/
	unsigned int GetViewDescCount() const;
private:
	/**
	*	保持しているConstantBufferView
	*/
	const D3D12_CONSTANT_BUFFER_VIEW_DESC mCbvView;

	/**
	*	Viewを何個作るかの数
	*/
	unsigned int mElementCount;

	/**
	*	ConstantBufferを作成するときのアライメント数
	*/
	const static unsigned int BUFFER_ALIGNMENT = 256;
};

