#pragma once
/**
*	@addtogroup Dx12PipelineState
*	@file PipelineStateObject.h
*	@brief ファイルの詳細
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/18
*/
#include "DirectX12/Shader/ShaderDatasStructure.h"

#include <string>
#include <d3d12.h>
#include <wrl.h>
#include <memory>

struct D3D12_SHADER_BYTECODE;
class RootSignatureObject;

/**
*	@ingroup Dx12PipelineState
*	@class PipelineStateObject
*	@brief ID3D12PipelineStateを作成し保持する
*/
class PipelineStateObject
{
public:
	/**
	*	@param[in]	name	GraphicsPipelineの名前
	*	@param[in]	gpsDesc	GraphicsPipelineのDesc
	*	@param[in]	dev		ID3D12Deviceの参照
	*/
	PipelineStateObject(const std::string& name, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	@param[in]	name	ComputePipelineの名前
	*	@param[in]	cpsDesc	ComputePipelineのDesc
	*	@param[in]	dev		ID3D12Deviceの参照
	*/
	PipelineStateObject(const std::string& name, D3D12_COMPUTE_PIPELINE_STATE_DESC& cpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	virtual ~PipelineStateObject();

	/**
	*	@brief	作成したPipelineStateを取得する
	*	@return	作瀬下PipelineState
	*/
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState() const;

	/**
	*	@brief PipelineStateを設定する
	*	@param[in] cmdList	設定するコマンドリスト
	*/
	void SetPipelineState(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList);

protected:
	PipelineStateObject();

	/**
	*	@brief	GraphicsPipelineStateを作成する
	*	@param[in]	name	PipelineStateの名前
	*	@param[in]	gpsDesc	GraphicsPipelineのDesc
	*	@param[in]	dev		ID3D12Deviceの参照
	*/
	void CreatePipelineState(const std::string& name, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	@brief	ComputePipelineStateを作成する
	*	@param[in]	name	PipelineStateの名前
	*	@param[in]	cpsDesc	ComputePipelineのDesc
	*	@param[in]	dev		ID3D12Deviceの参照
	*/
	void CreatePipelineState(const std::string& name, D3D12_COMPUTE_PIPELINE_STATE_DESC& cpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	@brief	InputElementを設定する
	*	@param[in,out]	gpsDesc	設定先のGraphicsPipelineState
	*	@param[in]		rootSignature	InputElementを持っているRootSignature
	*/
	void SetInputElement(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const std::shared_ptr<RootSignatureObject>& rootSignature);

	/**
	*	@brief	GraphicsPipelineのDescにシェーダーをセットする
	*	@param[in,out]	gpsDesc		シェーダーを登録したいGraphicsPipelineのDesc
	*	@param[in]	shaders		登録したいシェーダー
	*/
	void SetShaders(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const std::shared_ptr<ShaderDatas>& shaders);

	/**
	*	@brief	RootSignature周りに任せている設定をgpsに適応する
	*	@param[in,out]	gpsDesc	設定を適応する先のgpsDesc
	*	@param[in]		rootsignature	設定を保持しているRootSignature;
	*/
	void SetRootSignatureConfigure(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const std::shared_ptr<RootSignatureObject>& rootSignature);

	/**
	*	@brief	PipelineStateに名前をセットする
	*	@param[in]	name	PipelineStateの名前
	*/
	void SetName(const std::string& name);

	/**
	*	@brief	デフォルト設定のGraphicsPipelineStateを取得する
	*/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GetDefalutPipelineStateDesc() const;

	/**
	*	作成したPipelineState
	*/
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPipelineState;

	/**
	*	PipelineStateの名前
	*/
	std::string mName;
private:

	/**
	*	@brief	シェーダーを設定する
	*	@param[in,out]	byteCode	設定する先
	*	@param[in]		blob		設定するシェーダー
	*/
	void SetShader(D3D12_SHADER_BYTECODE& byteCode, ShaderResource& rsc);
};

