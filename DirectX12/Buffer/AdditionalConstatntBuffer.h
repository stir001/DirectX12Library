#pragma once
#include "ConstantBufferObject.h"
#include <vector>
/**
*	@file AdditionalConstantBuffer.h
*	@brief 後から追加するコンスタントバッファ
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/10
*/

#include <functional>
#include <d3d12.h>
#include <wrl.h>

/**
*	@ingroup Dx12Resoruce
*	@class AdditionalConstatntBuffer
*	@brief 後から追加するコンスタントバッファ
*/
template <typename T>
class AdditionalConstatntBuffer : public ConstantBufferObject
{
public:
	/**
	*	@param[in]	name	コンスタントバッファの名前
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	data	書き込むデータのポインタ
	*/
	AdditionalConstatntBuffer(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Device>& dev, T* data)
		: ConstantBufferObject(name, dev, sizeof(*data), 1),
		mData(data)
	{
		mWriteBuffer = [&]() {
			WriteBuffer256Alignment(mData, sizeof(*mData), 1);
		};
		mWriteBuffer();
		SetUpdate(mWriteBuffer);
	}

	/**
	*	@param[in]	name	コンスタントバッファの名前
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	data	書き込むデータのvector
	*/
	AdditionalConstatntBuffer(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::vector<T>* data)
		:ConstantBufferObject(name, dev, sizeof(data[0], data.size())),
		mVectorDatas(data)
	{
		mWriteBuffer = [&]() {
			WriteBuffer256Alignment(&(*mVectorDatas)[0], sizeof((*mVectorDatas)[0]), (*mVectorDatas).size());
		};

		mWriteBuffer();
		SetUpdate(mWriteBuffer);
	}

	~AdditionalConstatntBuffer()
	{
	}

private:
	/**
	*	コンスタントバッファに書き込むためのラムダ式
	*/
	std::function<void(void)> mWriteBuffer;

	/**
	*	書き込むデータ
	*/
	T* mData;

	/**
	*	書き込むデータのvector
	*/
	std::vector<T>* mVectorDatas;
};

