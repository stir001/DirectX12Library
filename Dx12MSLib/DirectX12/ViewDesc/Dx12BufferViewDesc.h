#pragma once
/**
*	@addtogroup Dx12BufferView
*	@file ファイル名
*	@brief ファイルの詳細
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/11
*/
#include <d3d12.h>
#include <wrl.h>

/**
*	@ingroup Dx12BufferView
*	@class Dx12BufferViewDesc
*	@brief D3D12のViewDescを持つクラスの基底クラス
*/
class Dx12BufferViewDesc
{
public:
	Dx12BufferViewDesc();
	virtual ~Dx12BufferViewDesc();

	/**
	*	@brief	ViewDescが何個分あるかを返す オーバーライドしない場合必ず1を返す
	*	@return		ViewDescの総数
	*/
	virtual unsigned int GetViewDescCount() const;

	/**
	*	@brief	ViewDescをDescreptorHeapの任意の場所にセットする
	*	@param[in]	dev	ID3D12Deviceの参照
	*	@param[in]	cpuHandle	DescriptorHeapのCPUHandle
	*	@param[in]	gpuHandle	DescriptorHeapのGPUHandle
	*	@param[in]	resource	必要な場合のみViewに必要なID3D12Resource デフォルトはnullptr
	*/
	virtual void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr) = 0;
};

