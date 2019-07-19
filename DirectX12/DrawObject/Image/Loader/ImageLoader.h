#pragma once
/**
*	@file ImageLoader.h
*	@brief 2D画像をロードしそれを保持、管理するクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/12/19
*/

#include "DirectX12/Shader/ShaderDatasStructure.h"
#include "DirectX12/DrawObject/DrawObjectLoader.h"

#include <string>
#include <map>
#include <memory>
#include <d3d12.h>

class ImageController;
class Image3DController;
class ImageObject;
class RootSignatureObject;
class PipelineStateObject;

/**
*	@ingroup DrawObjectLoader
*	@class ImageLoader
*	@brief 2D画像をロードしてそれを保持、管理する
*			保持しているものをもう一度ロードしようとした場合すでにロードしているものを使って処理をする
*			データを解放するためにはRelease関数を呼ぶ
*/
class ImageLoader : public DrawObjectLoader
{
public:
	~ImageLoader();

	/**
	*	@brief	2D画像をロードし、2Dとして操作するコントローラーを返す
	*	@param[in]	path	ロードする2D画像パス
	*/
	std::shared_ptr<ImageController> LoadImage2D(const std::string& path);

	/**
	*	@brief	2D画像をロードし、3Dとして操作するコントローラーを返す
	*	@param[in]	path	ロードする2D画像パス
	*/
	std::shared_ptr<Image3DController> LoadImage3D(const std::string& path, bool isBillboard = false);
	
	/**
	*	@brief UIレイヤーのコマンドリストを設定する
	*	@param[in]	cmdList	設定するコマンドリスト
	*/
	void SetUIPassCommandList(std::shared_ptr<Dx12CommandList>& cmdList);

	/**
	*	@brief BackGroundレイヤーのコマンドリストを設定する
	*	@param[in]	cmdList	設定するコマンドリスト
	*/
	void SetBackGroundPassCommandList(std::shared_ptr<Dx12CommandList>& cmdList);

	/**
	*	@brief Modelレイヤーのコマンドリストを設定する
	*	@param[in]	cmdList	設定するコマンドリスト
	*/
	void Set3DPassCommandList(std::shared_ptr<Dx12CommandList>& cmdList);

	/**
	*	@brief	指定されたファイルパスでロードしたオブジェクトの所有権を手放す
	*	@param[in]	releaseImagePath	解放したいファイルパス
	*/
	void Release(const std::string& releaseImagePath);

	/**
	*	@brief	ImageLodaerの参照を取得する
	*	@return	ImageLoaderの参照
	*/
	static  ImageLoader& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new ImageLoader();
		}
		return *mInstance;
	}

	/**
	*	@brief	ImageLoaderを破棄する
	*/
	static void Destroy()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}
private:
	ImageLoader();
	ImageLoader(const ImageLoader&);
	ImageLoader(const ImageLoader&&);
	ImageLoader& operator=(const ImageLoader&);

	/**
	*	インスタンスの実態保持用
	*/
	static ImageLoader* mInstance;

	/**
	*	2D画像情報管理用
	*/
	std::map<std::string, std::shared_ptr<ImageObject>> mImages;

	/**
	*	背景レイヤー用のコマンドリスト
	*/
	std::shared_ptr<Dx12CommandList> mBackCmdList;

	/**
	*	3D描画レイヤー用のコマンドリスト
	*/
	std::shared_ptr<Dx12CommandList> mModelCmdList;
	
	/**
	*	2D画像描画に使用するrootsiganture
	*/
	std::shared_ptr<RootSignatureObject> mRootsignature;

	/**
	*	2D画像描画に使用するpipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mPipelinestate;

	/**
	*	2D画像を3Dとして描画するときに使用するrootsiganaure
	*/
	std::shared_ptr<RootSignatureObject> m3DRootsignature;

	/**
	*	2D画像を3Dとして描画するときに使用するrootsignature
	*/
	std::shared_ptr<PipelineStateObject> m3DPipelinestate;

	/**
	*	2D画像を3Dのビルボードとして描画するときに使用するrootsignature
	*/
	std::shared_ptr<RootSignatureObject> mBillboardRootsignature;

	/**
	*	2D画像を3Dのビルボードとして描画するときに使用するpipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mBillboardPipelineState;

	/**
	*	2D画像描画用のシェーダー情報
	*/
	std::shared_ptr<ShaderDatas> mShader;

	/**
	*	2D画像を3Dとして描画するときのシェーダ情報
	*/
	std::shared_ptr<ShaderDatas> m3DShader;

	/**
	*	@brief	pipelinestateを作成する
	*	@param[in]	dev		ID3D12Deviceの参照
	*/
	void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	@brief	rootsignatureを作成する
	*	@param[in]	dev		ID3D12Deviceの参照
	*/
	void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
};

