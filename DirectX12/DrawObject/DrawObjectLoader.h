#pragma once
/**
*	@addtogroup DrawObjectLoader
*	@file DrawObjectLoader
*	@brief 描画オブジェクトをロードする基底クラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/14
*/

#include "DirectX12/Shader/ShaderDatasStructure.h"

#include <string>
#include <memory>
#include <d3d12.h>

class File;
class ShaderCompiler;
class PipelineStateObject;
class RootSignatureObject;
class Dx12CommandList;
struct ShaderDatas;


/**
*	@ingroup DrawObjectLoader
*	@class DrawObjectLoader
*	@brief 描画オブジェクトをロードする基底クラス
*/
class DrawObjectLoader
{
public:
	DrawObjectLoader();
	virtual ~DrawObjectLoader();

	/**
	*	@brief	ローダーでロードしたすべてのオブジェクトに渡すコマンドリストを設定する
	*	@param[in]	cmdList		オブジェクトに渡すコマンドリスト
	*/
	void SetRenderingCommnadList(std::shared_ptr<Dx12CommandList>& cmdList);
protected:
	/**
	*	ロードするときに使用するファイル
	*/
	std::shared_ptr<File> mFp;

	/**
	*	使用しているファイルへの相対パス保存用
	*/
	std::string mRelativePath;

	/**
	*	使用するシェーダー情報保存用
	*/
	std::shared_ptr<ShaderDatas> mShader;

	/**
	*	ロードしたオブジェクトに渡すpipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mPipelinestate;

	/**
	*	ロードしたオブジェクトに渡すrootsignature
	*/
	std::shared_ptr<RootSignatureObject> mRootsignature;

	/**
	*	ロードしたオブジェクトに渡すcommandlist
	*/
	std::shared_ptr<Dx12CommandList> mCmdList;

	/**
	*	@brief	渡されたファイルパスの相対パスをmReletivePath変数に保存する
	*/
	void GetRelativePath(const std::string& path);

	/**
	*	pipelinestateを作成する
	*/
	virtual void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev) = 0;

	/**
	*	rootsigantureを作成する
	*/
	virtual void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev) = 0;
private:

};