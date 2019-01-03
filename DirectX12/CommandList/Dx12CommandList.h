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
#include <vector>

class Dx12BufferObject;
class Dx12DescriptorHeapObject;
class DrawObjectController;
class RootSignatureObject;
class PipelineStateObject;
class IndexBufferObject;
class VertexBufferObject;

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
	virtual HRESULT Reset();

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
	HRESULT SetDescriptorHeap(const std::shared_ptr<const Dx12DescriptorHeapObject>& descHeap) const;

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

	/**
	*	描画オブジェクトを登録する
	*/
	void SetDrawController(std::shared_ptr<DrawObjectController> controller);

	/**
	*	登録したコントローラーを開放する
	*/
	void ClearControllers();

	/**
	*	バンドルコマンドリストのコマンドを積み込む
	*/
	void ExecuteBundle(std::shared_ptr<Dx12CommandList>& bundle);

	/**
	*	@brief	pipelineStateを設定する
	*	@param[in]	pipelineState	設定するpipelineState
	*/
	void SetPipelineState(const std::shared_ptr<PipelineStateObject>& pipelineState);

	/**
	*	@brief	rootSignatureを設定する
	*	@param[in]	rootSignature	設定するrootSignature
	*/
	void SetGraphicsRootSignature(const std::shared_ptr<RootSignatureObject>& rootSignature);

	/**
	*	@brief	IndexBufferを設定する
	*	@param[in]	indexBuffer	設定するIndexBuffer
	*/
	void IASetIndexBuffer(const std::shared_ptr<IndexBufferObject>& indexBuffer);

	/**
	*	@brief	VertexBufferを設定する
	*	@param[in]	vertexBuffers	設定するvertexBufferの配列
	*	@param[in]	vertexBufferNum	設定するvertexBufferの数
	*/
	void IASetVertexBuffers(const std::shared_ptr<VertexBufferObject> vertexBuffers[], unsigned int vertexBufferNum);

	/**
	*	@brief	primitive topology を設定する
	*	@param[in]	primitiveTopology	設定するprimitive tpopology
	*/
	void IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY primitiveTopology);

	/**
	*	@brief	lndexを使ってインスタンスを指定個数生成する
	*	@param[in]	indexNum	インスタンス一つ当たりのインデックスの数
	*	@param[in]	instanceNum	生成するインスタンスの数
	*	@param[in]	startIndexLocation	使用するインデックスの開始する場所のオフセット
	
	*/
	void DrawIndexedInstanced(unsigned int indexNum, unsigned int instanceNum
		, unsigned int startIndexLocation = 0, unsigned int baseVertexLocation = 0, unsigned int startInstanceLocation = 0);

	/**
	*	@brief	Vertexのみでインスタンスを指定個数生成する
	*	@param[in]	vertexNum	インスタンス一つ当たりのvertexの数
	*	@param[in]	instanceNum	生成するインスタンスの数
	*	@param[in]	startVertexLocation 
	*/
	void DrawInstanced(unsigned int vertexNum, unsigned int instanceNum, unsigned int startVertexLocation, unsigned int startInstanceLocation);

	/**
	*	@brief	buffer(resource)の中身をすべてコピーする
	*	@param[in]	dst	コピー先
	*	@param[in]	src	コピー元
	*/
	void CopyResource(std::shared_ptr<Dx12BufferObject> dst, std::shared_ptr<Dx12BufferObject> src);
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

	std::vector<std::shared_ptr<DrawObjectController>> mControllers;

	/**
	*	@brief	ID3D12DeviecがRemoveされたかどうかをチェックする
	*	@return	結果
	*/
	HRESULT GetDeviceRemoveReason() const;
};

