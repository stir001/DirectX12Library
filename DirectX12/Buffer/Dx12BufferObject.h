#pragma once
/**
*	@addtogroup Dx12Resource
*	@file Dx12BufferObject.h
*	@brief ID3D12Resourceを扱うクラスの定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/10
*/

#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <string>

class Dx12BufferViewDesc;

/**
*	@ingroup Dx12Resource
*	@class Dx12BufferObject
*	@brief ID3D12Resourceを扱う基底クラス
*			各ビューを作るための関数を持ち必要に応じて呼び出すことでviewを作成する
*/
class Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	ID3D12Resourceにつける名前
	*/
	Dx12BufferObject(const std::string& name);

	/**
	*	@param[in]	rsc				あらかじめ作っておいたこのクラスにセットしたいResource
	*	@param[in]	name			ID3D12Resourceにつける名前
	*	@param[in]	elementSize		要素一個のbyte単位のサイズ
	*	@param[in]	elementCount	要素が何個あるのか
	*/
	Dx12BufferObject(Microsoft::WRL::ComPtr<ID3D12Resource>& rsc, const std::string& name,
		unsigned int elemenetSize, unsigned int elementCount);
	virtual ~Dx12BufferObject();

	/**
	*	@brief	pDataの要素をID3D12Resourceにマップする
	*	@param[in]	pData			書き込むデータのポインタ
	*	@param[in]	amountDataSize	data全体のバイト単位のサイズ
	*/
	void WriteBuffer(const void* pData, unsigned int amountDatasSize);

	/**
	*	@brief	pDataの要素をID3D12Resourceに要素ごとに256アライメントしながらマップする
	*	@param[in]	pData		データのポインタ
	*	@param[in]	dataSize	データ一個のサイズ
	*	@param[in]	dataCount	データの総数
	*/
	void WriteBuffer256Alignment(const void* pData, unsigned int dataSize, unsigned int dataCount);

	/**
	*	D3D12_CONSTANT_BUFFER_VIEW_DESCを作成する
	*/
	virtual void CreateConstantBufferViewDesc();

	/**
	*	D3D12_UNORDERD_ACCESS_VIEW_DESCを作成する
	*/
	virtual void CreateUnorderdAccessViewDesc();

	/**
	*	D3D12_SHADER_RESOURCE_VIEW_DESCを作成する
	*/
	virtual void CreateShaderResourceViewDesc();

	/**
	*	D3D12_RENDER_TARGET_VIEW_DESCを作成する
	*/
	virtual void CreateRenderTargetViewDesc();

	/**
	*	D3D12_DEPTH_STENCIL_VIEW_DESCを作成する
	*/
	virtual void CreateDepthStecilViewDesc();

	/**
	*	@brief	保持しているID3D12Resourceの参照を取得する
	*	@return	ID3D12ResourceのComPtrの参照
	*/
	Microsoft::WRL::ComPtr<ID3D12Resource>& GetBuffer();

	/**
	*	@brief	現在のD3D12_RESOURCE_STATESを取得する
	*	@return	現在のD3D12_RESOURCE_STATES
	*/
	D3D12_RESOURCE_STATES GetCurrentState() const;

	/**
	*	@brief	D3D12_RESOURCE_STATESを設定する
	*	@param[in]	state	設定するD3D12_RESOURCE_STATES
	*/
	void SetCurrentState(D3D12_RESOURCE_STATES state);

	/**
	*	@brief	初期状態のD3D12_RESOURCE_STATESを取得する
	*	@return	初期状態のD3D12_RESOURCE_STATES
	*/
	D3D12_RESOURCE_STATES GetDefaultState() const;

	/**
	*	@brief	作成したViewDescを取得する
	*	@return	作成したDx12BufferViewDesc
	*/
	const std::shared_ptr<Dx12BufferViewDesc>& GetViewDesc() const;

	/**
	*	@brief	バッファの名前を取得する
	*	@return	バッファの名前
	*/
	std::string GetName() const;

protected:
	/**
	*	保持しているID3D12Resource
	*/
	Microsoft::WRL::ComPtr<ID3D12Resource> mBuffer;

	/**
	*	現在のステート
	*/
	D3D12_RESOURCE_STATES mCurrentState;

	/**
	*	初期ステート
	*/
	D3D12_RESOURCE_STATES mDeafultState;

	/**
	*	作成したViewDesc
	*/
	std::shared_ptr<Dx12BufferViewDesc> mViewDescs;

	/**
	*	ID3D12Resourceの名前
	*/
	std::wstring mName;

	/**
	*	要素一個のbyte単位のサイズ
	*/
	unsigned int mElementSize;

	/**
	*	要素の総数
	*/
	unsigned int mElementCount;

	/**
	*	ID3D12Resourceをクリアするときに値
	*/
	D3D12_CLEAR_VALUE mClearValue;

#ifdef _DEBUG
	/**
	*	デバッグ情報用　プロパティ一時保存
	*/
	D3D12_HEAP_PROPERTIES mHeapProp;

	/**
	*	デバッグ情報用　Desc一時保存用
	*/
	D3D12_RESOURCE_DESC mRscDesc;
#endif
	/**
	*	要素をマップするときに使用する領域
	*/
	void* mElementBuffer;

	/**
	*	要素をマップする
	*/
	virtual void Map();
};

