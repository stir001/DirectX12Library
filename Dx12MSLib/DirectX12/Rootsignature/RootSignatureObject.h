#pragma once
/**
*	@addtogroup Dx12RootSignature
*	@file RootSignatureObject.h
*	@brief ID3D12RootSignatureを作成し保持するクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/18
*/
#include "Shader/ShaderDatasStructure.h"

#include <vector>
#include <d3dcommon.h>
#include <wrl.h>
#include <d3d12.h>

struct ID3D12RootSignature;
struct ID3D10Blob;

/**
*	@ingroup Dx12RootSignature
*	@class RootSignatureObject
*	@brief ID3D12RootSignatureを作成し保持する
*/
class RootSignatureObject
{
public:
	/**
	*	@param[in]	signatureBlob	rootsigantureを定義している情報を持つblob
	*	@param[in]	dev		ID3D12Deviceの参照
	*/
	RootSignatureObject(const std::string& name,ID3D10Blob* signatureBlob, Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	virtual ~RootSignatureObject();

	/**
	*	@brief	使用するシェーダーを設定する
	*	@param[in]	shader	設定するシェーダー
	*/
	void SetShaderData(const ShaderDatas& shader);

	/**
	*	@brief	使用しているシェーダーを取得する
	*	@return	使用しているシェーダー
	*/
	ShaderDatas& GetShaderDatas();

	/**
	*	@brief	ID3D12RootSignatureを取得する
	*	@return	作成したrootsignature
	*/
	Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetRootSignature();
protected:
	RootSignatureObject();

	/**
	*	@brief	rootsignatureを作成する
	*	@param[in]	signatureBlob	rootsignatureの情報をもつblob
	*	@param[in]	dev		ID3D12Deviceの参照
	*/
	void CreateRootSignature(const std::string& name, ID3D10Blob* signatureBlob, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	作成したrootsigantrue
	*/
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;

	/**
	*	使用するシェーダー
	*/
	ShaderDatas mShader;
private:
};

