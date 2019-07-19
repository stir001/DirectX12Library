#pragma once
/**
*	@file PMDLoader.h
*	@brief PMDモデルの情報をロードし保持するクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/12/12
*/
#include "DirectX12/DrawObject/DrawObjectLoader.h"
#include <map>

class PMDModel;
class PMDController;
class DirectionalLight;

/**
*	@ingroup DrawObjectLoader
*	@class PMDLoader
*	@brief PMDをロードし保持するクラス
*/
class PMDLoader :
	public DrawObjectLoader
{
public:
	PMDLoader();
	~PMDLoader();

	/**
	*	@brief	PMDをロードしPMDを操作するコントローラーを返す
	*	@param[in]	path	ロードするPMDモデルのファイルパス
	*	@return	PMDを操作するコントローラー
	*/
	std::shared_ptr<PMDController> Load(const std::string& path);
private:

	/**
	*	ロードしたモデルに適応するライト
	*/
	std::shared_ptr<DirectionalLight> mLight;

	/**
	*	ロードしたモデル保存用
	*/
	std::map<std::string, std::shared_ptr<PMDModel>> mModels;

	/**
	*	現在ロードしているモデルの一時保存用
	*/
	std::shared_ptr<PMDModel> mLoadingmodel;

	/**
	*	テクスチャを使って描画するときに使うシェーダー情報
	*/
	std::shared_ptr<ShaderDatas> mSubShader;

	/**
	*	Toonテクスチャを使って描画するときに使うrootsiganture
	*/
	std::shared_ptr<RootSignatureObject> mToonRootsiganture;

	/**
	*	Toonテクスチャを使って描画するときに使うPipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mToonPipelineState;

	/**
	*	Header部分をロード
	*/
	void LoadHeader();

	/**
	*	頂点情報部分をロード
	*/
	void LoadVertex();

	/**
	*	インデックス情報をロード
	*/
	void LoadIndex();

	/**
	*	マテリアル情報をロード
	*/
	void LoadMaterial();

	/**
	*	ボーン情報をロード
	*/
	void LoadBone();

	/**
	*	IK情報をロード
	*/
	void LoadIkList();

	/**
	*	表情情報をロード
	*/
	void LoadSkin();

	/**
	*	表情表示情報をロード
	*/
	void LoadSkinDisp();

	/**
	*	ボーンフレーム表示情報をロード
	*/
	void LoadBoneFrameDisp();

	/**
	*	ボーン情報をロード
	*/
	void LoadBoneDisp();

	/**
	*	英語の除法をスキップ
	*/
	void SeekEngligh();

	/**
	*	トゥーン情報をロード
	*/
	void LoadToonPath();

	/**
	*	剛体情報をロード
	*/
	void LoadRigidBody();

	/**
	*	ジョイント情報をロード
	*/
	void LoadJoint();

	/**
	*	モデルに対応したIndexBufferを作成
	*/
	void CreateIndexBuffer();

	/**
	*	モデルに対応したVertexBufferを作成
	*/
	void CreateVertexBuffer();

	/**
	*	テクスチャをロードしShaderResourceを作成
	*/
	void CreateTexture();

	/**
	*	toonTextureを作成する
	*/
	void CreateToonTexture();

	/**
	*	マテリアル用のバッファを作成
	*/
	void CreateMaterialBuffer();

	/**
	*	@brief	ロードしたモデルに使用するPipelinestateを作成
	*	@param[in]	dev		ID3D12Deviceの参照
	*/
	void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	@brief	ロードしたモデルに使用するRootsignatureを作成
	*	@param[in]	dev		ID3D12Deviceの参照
	*/
	void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	@brief	PMDモデルを操作するクラスの作成
	*	@param[in]	model	コントロールするモデル
	*	@param[in]	path	モデルのロードときに指定されたファイルパス
	*	@return	PMDモデルを操作するコントローラー
	*/
	std::shared_ptr<PMDController> CreateController(std::shared_ptr<PMDModel>& model, const std::string& path);

	/**
	*	@brief	指定されたパスのモデルの名前を得る
	*	@param[in]	path	モデルの名前が欲しいモデルへのファイルパス
	*	@return	モデルの名前
	*/
	std::string GetModelName(const std::string& path) const;

	void CreateRigidBodis();
	
};