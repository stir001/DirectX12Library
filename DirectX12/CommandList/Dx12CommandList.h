#pragma once
/**
*	@addtogroup Dx12CommandList
*	@file Dx12CommandList.h
*	@brief Dx12GraphicsCommandListをラップしたクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/11
*/

#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <string>

class Dx12BufferObject;
class Dx12DescriptorHeapObject;

/**
*	@ingroup Dx12CommandList
*	@class Dx12CommandList
*	@brief ID3D12GraphicsCommandListをラップしたクラス
*/
class Dx12CommandList
{
public:
	/**
	*	@param	name	コマンドリストの名前
	*	@param	dev		ID3D12Deviceの参照
	*	@param	type	コマンドリストのタイプ
	*/
	Dx12CommandList(const std::string& name 
		, const Microsoft::WRL::ComPtr<ID3D12Device>& dev 
		, D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
	virtual ~Dx12CommandList();
	
	/**
	*	@brief	コマンドリストを取得する
	*	@return 保持しているコマンドリスト
	*/
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList() const;

	/**
	*	@brief	コマンドリストのタイプを得る
	*	@return	コマンドリストタイプ
	*/
	D3D12_COMMAND_LIST_TYPE GetType() const;

	/**
	*	@brief	コマンドリストをリセットする
	*	@return	リセットの結果
	*/
	virtual HRESULT Reset() const;

	/**
	*	@brief	コマンドリストを任意のPipelineStateを使ってリセットする
	*	@param[in]	pipelineState	リセット時に使用するpipelinestate
	*	@return	リセットの結果
	*/
	virtual HRESULT Reset(Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState);

	/**
	*	@brief	CommandListに任意のDescriptorHeapをセットする
	*	@param[in]	descHeap	セットするDescriptorHeapをラップしたもの
	*	@return	結果
	*/
	HRESULT SetDescriptorHeap(const std::shared_ptr<Dx12DescriptorHeapObject>& descHeap) const;

	/**
	*	@brief	CommandListに任意のDescriptorHeapをセットする
	*	@param[in]	descHeap	セットするDescriptorHeap
	*	@return	結果
	*/
	HRESULT SetDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descHeap) const;

	/**
	*	@brief	DescriptorHeapの任意の場所をテーブルにセットする
	*	@param[in]	descHeap	セットしたい要素を持つDescriptorHeap
	*	@param[in]	resourceIndex	セットしたいDescriptorHeapのIndex
	*	@param[in]	rootparamIndex	セットするテーブルのインデックス
	*	@return	結果
	*/
	HRESULT SetGraphicsRootDescriptorTabel(std::shared_ptr<Dx12DescriptorHeapObject>& descHeap, int resourceIndex, int rootparamIndex) const;

	/**
	*	@brief	DescriptorHeapのgpuHandleの場所をテーブルにセットする
	*	@param[in]	rootparamaterIndex	セットするテーブルのインデックス
	*	@param[in]	gpuHandle			DescriptorHeapのセットしたい場所のgpuHandle
	*	@return	結果
	*/
	HRESULT SetGraphicsRootDescriptorTable(int rootparamaterIndex, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle) const;

	/**
	*	@brief	TransitionBarrierをはる
	*	@param[in]	resource	バリアを張りたいリソース
	*	@param[in]	transitionState	どのステートにトランジションするかのステート
	*	@return	結果
	*/
	HRESULT TransitionBarrier(std::shared_ptr<Dx12BufferObject>& resource, D3D12_RESOURCE_STATES transitionState) const;
	
	/**
	*	@brief	DepthStencilを指定の値で埋める
	*	@param[in]	depthCpuHandle	DepthStencilViewのあるcpuHandle
	*	@param[in]	clearValue		埋めたい値
	*	@param[in]	rect			どの範囲を埋めるか
	*	@param[in]	rectNum			何個Rectを渡したか
	*/
	void ClearDepthView(D3D12_CPU_DESCRIPTOR_HANDLE depthCpuHandle, float clearValue ,D3D12_RECT* rect, unsigned int rectNum = 0);

	/**
	*	@brief	DepthStencilを指定の値で埋める
	*	@param[in]	depthCpuHandle	DepthStencilViewのあるcpuHandle
	*	@param[in]	clearValue		埋めたい値
	*	@param[in]	rect			どの範囲を埋めるか
	*	@param[in]	rectNum			何個Rectを渡したか
	*/
	void ClearStencilView(D3D12_CPU_DESCRIPTOR_HANDLE stencilCpuHandle, unsigned int clearValue, D3D12_RECT* rect, unsigned int rectNum = 0);

	/**
	*	@brief	RenderTargetをセットする
	*	@param[in]	renderTargetCount	なんこRenderTargetがあるか
	*	@param[in]	renderTargetCpuHandle	RenderTargetViewがあるDescriptorのCPUHandle
	*	@param[in]	depthCpuHandle		レンダーターゲットに書き込むときに使うDepthSrencilViewのあるDescriptorHeapのCPUHandle
	*/
	void OMSetRenderTargets(unsigned int renderTargetCount, D3D12_CPU_DESCRIPTOR_HANDLE rendertargetCpuHandle, D3D12_CPU_DESCRIPTOR_HANDLE* depthCpuHandle = nullptr);

	/**
	*	@brief	レンダーターゲットを指定の値で埋めてclearする
	*	@param[in]	rendertargetCpuHandle	RenderTaregetViewのあるDescriptorHeapのCpuHandle
	*	@param[in]	colorRGBA		埋める値
	*	@param[in]	rect 	どの範囲を埋めるか
	*	@param[in]	rectNum	Rectを何個渡したか
	*/
	void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rendertargetCpuHandle, const float colorRGBA[4], D3D12_RECT* rect, unsigned int rectNum = 0);

	/**
	*	@brief ViewPortをセットする
	*	@param[in]	viewPorts	セットするViewPort
	*	@param[in]	viewPortsCount	ViewPortの総数
	*/
	void RSSetViewports(D3D12_VIEWPORT* viewPorts, unsigned int viewportCount = 1);

	/**
	*	@brief	シザーレクトをセットする
	*	@param[in]	scissorRects	セットするシザーレクト
	*	@param[in]	scissorRectCountシザーレクトの総数
	*/
	void RSSetScissorRects(D3D12_RECT* scissorRects, unsigned int scissorRectsCount = 1);

	/**
	*	@brief	コマンドリストの名前を得る
	*	@return	コマンドリストの名前
	*/
	const std::string& GetName() const;

	/**
	*	コマンドリストを閉じる
	*/
	void Close() const;
private:
	/**
	*	コマンドリストの名前
	*/
	std::string mName;

	/**
	*	保持しているコマンドリスト
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;

	/**
	*	保持しているコマンドアロケーター
	*/
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCmdallcator;

	/**
	*	コマンドリストのタイプ
	*/
	const D3D12_COMMAND_LIST_TYPE mType;

	/**
	*	@brief	ID3D12DeviecがRemoveされたかどうかをチェックする
	*	@return	結果
	*/
	HRESULT GetDeviceRemoveReason() const;
};

