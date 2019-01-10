#pragma once
/**
*	@addtogroup	Dx12DescriptorHeap
*	@file Dx12DescriptorHeapObject
*	@brief ID3D12DescriptorHeapをラップし保持するクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/12
*/

#include <wrl.h>
#include <vector>
#include <memory>
#include <d3d12.h>

struct ID3D12DescriptorHeap;
struct ID3D12Device;
class Dx12BufferObject;
class Dx12CommandList;

/**
*	@ingroup Dx12DescriptorHeap
*	@class Dx12DescriptorHeapObject
*	@brief ID3D12DescriptorHeapを保持する
*/
class Dx12DescriptorHeapObject : public std::enable_shared_from_this<Dx12DescriptorHeapObject>
{
public:
	/**
	*	@param[in]	name		DescriptorHeapの名前
	*	@param[in]	dev			ID3D12Deviceの参照
	*	@param[in]	buffers		DescriptorHeapにセットするBufferのvector
	*	@param[in]	heapType	DescriptorHeapのタイプ
	*/
	Dx12DescriptorHeapObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		std::vector<std::shared_ptr<Dx12BufferObject>>& buffers, D3D12_DESCRIPTOR_HEAP_TYPE heapType);
	virtual ~Dx12DescriptorHeapObject();

	/**
	*	@brief	DescriptorHeapを取得する
	*	@return	保持しているDescriptorHeap
	*/
	virtual Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap()const;

	/**
	*	@brief	GetCPUDescriptorHandleForHeapStart()のラップ関数
	*	@return	DescriptorHeapのCPUハンドルの最初の位置
	*/
	virtual D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHeapHandleStart() const;

	/**
	*	@brief	GetGPUDescriptorHandleForHeapStart()のラップ関数
	*	@return	DescriptorHeapのGPUハンドルの最初の位置
	*/
	virtual D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHeapHandleStart() const;

	/**
	*	@brief	DescriptorHeapをコマンドリストにセットする
	*	@param[in]	cmdList	DescriptorHeapをセットするコマンドリスト
	*/
	virtual void SetDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const;

	/**
	*	@brief	DescriptorHeapをコマンドリストにセットする
	*	@param[in]	cmdList	DescriptorHeapをセットするコマンドリスト
	*/
	virtual void SetDescriptorHeap(const std::shared_ptr<Dx12CommandList>& cmdList) const;

	/**
	*	@brief	コマンドリストにDescriptorHeapTableを設定する
	*	@param[in]	resourceHandleIndex	DescriptorTableに格納しているリソースハンドルのインデックス
	*	@param[in]	rootParamaterIndex	リソースをバインドするrootParamaterIndex
	*	@param[in]	handleOffsetCount	resourceHandleのオフセット(何個目を使用するか)
	*/
	virtual void SetGraphicsDescriptorTable(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, unsigned int resourceHandleIndex, unsigned int rootParamaterIndex, unsigned int handleOffsetCount = 0) const;

	/**
	*	@brief	コマンドリストにDescriptorHeapTableを設定する
	*	@param[in]	resourceHandleIndex	DescriptorTableに格納しているリソースハンドルのインデックス
	*	@param[in]	rootParamaterIndex	リソースをバインドするrootParamaterIndex
	*	@param[in]	handleOffsetCount	resourceHandleのオフセット(何個目を使用するか)
	*/
	virtual void SetGraphicsDescriptorTable(const std::shared_ptr<Dx12CommandList> cmdList, unsigned int resourceHandleIndex, unsigned int rootParamaterIndex, unsigned int handleOffsetCount = 0) const;
protected:
	/**
	*	保持しているDescriptorHeap
	*/
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescHeap;
private:
	/**
	*	DescriptorHeapと関連づけられているリソースの情報
	*/
	struct ResourceBind
	{
		std::weak_ptr<Dx12BufferObject> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	};

	/**
	*	HeapTypeに応じたインクリメントの値
	*/
	UINT mHeapIncrementSize;

	/**
	*	このクラスと関連付けられているResourceの情報
	*/
	std::vector<ResourceBind> mResourceBinds;
};

