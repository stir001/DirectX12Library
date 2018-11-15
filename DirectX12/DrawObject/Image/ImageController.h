#pragma once
/**
*	@file ImageController.h
*	@brief 2D画像の操作を行うクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/14
*/
#include "DrawObject/DrawObjectController.h"
#include "ImageControllerStructure.h"

#include <memory>
#include <DirectXMath.h>

class ImageObject;
class Rect;
class VertexBufferObject;
class Dx12DescriptorHeapObject;

/**
*	@ingroup DrawObjectController
*	@class ImageController
*	@brief 2D画像を操作するクラス
*/
class ImageController :public DrawObjectController
{
public:
	/**
	*	@param[in]	img		2D画像情報
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	cmdList	コマンドを積むコマンドリスト
	*	@param[in]	pipelinestate	使用するpipelinestate
	*	@param[in]	rootsignature	使用するrootsignature
	*/
	ImageController(std::shared_ptr<ImageObject> img,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList,
		std::shared_ptr<PipelineStateObject>& pipelinestate,
		std::shared_ptr<RootSignatureObject>& rootsignature);

	~ImageController();

	/**
	*	@brief	画像を平行移動させる
	*	@param[in]	x	xの平行移動成分
	*	@param[in]	y	yの平行移動成分
	*	@param[in]	z	zの平行移動成分
	*/
	void AddPos(const float x, const float y, const float z);

	/**
	*	@brief	画像を平行移動させる
	*	@param[in]	offset	平行移動線分
	*/
	void AddPos(const DirectX::XMFLOAT3& offset);

	/**
	*	@brief	画像の拡大率を変化させる
	*	@param[in]	scale	画像の拡大率の変化量
	*/
	void AddScale(const float scale);

	/**
	*	@brief	画像を回転させる
	*	@param[in]	deg		回転量(degree,度数法)
	*/
	void AddRota(const float deg);

	/**
	*	@brief	画像の基準点を平行移動させる
	*	@param[in]	offset	平行移動成分
	*/
	void AddPivot(const DirectX::XMFLOAT3& offset);

	/**
	*	@brief	画像の中の任意の矩形部分を描画する部分を設定する
	*	@param[in]	inc		描画したい矩形の中心点
	*	@param[in]	inw		矩形の縦幅
	*	@param[in]	inh		矩形の縦幅
	*/
	void SetRect(const DirectX::XMFLOAT3& inc, const float inw, const float inh);

	/**
	*	@brief	画像の中の任意の矩形部分を描画する部分を設定する
	*	@param[in]	rc		描画したい矩形の情報
	*/
	void SetRect(const Rect& rc);

	/**
	*	@brief	画像の基準点を任意の場所へ平行移動させる
	*	@param[in]	x		xの平行移動成分
	*	@param[in]	y		yの平行移動成分
	*	@param[in]	z		zの平行移動成分
	*/
	void SetPos(const float x, const float y, const float z);

	/**
	*	@brief	画像の基準点を任意の場所へ平行移動させる
	*	@param[in]	offset	平行移動成分
	*/
	void SetPos(const DirectX::XMFLOAT3& offset);

	/**
	*	@brief	画像の拡大率を等倍で設定する
	*	@param[in]	scale	設定する拡大率
	*/
	void SetScale(const float scale);

	/**
	*	@brief	画像の拡大率を縦横個別に設定する
	*	@param[in]	scale	設定する拡大率(scale.x : u方向の拡大率、scale.y : v方向の拡大率)
	*/
	void SetScale(const DirectX::XMFLOAT2& scale);

	/**
	*	@brief	画像の回転を設定する
	*	@param[in]	deg		設定したい回転角度(degree,度数法)
	*/
	void SetRota(const float deg);

	/**
	*	@brief	画像の中心点の基準点からのオフセットを設定する
	*	@param[in]	x		x成分のオフセット量
	*	@param[in]	y		y成分のオフセット量
	*	@param[in]	z		z成分のオフセット量
	*/
	void SetCenterOffset(const float x, const float y, const float z);

	/**
	*	@brief	画像の中心点の基準点からのオフセットを設定する
	*	@param[in]	offset	オフセット量
	*/
	void SetCenterOffset(const DirectX::XMFLOAT3& offset);

	/**
	*	U方向に反転させる
	*/
	void TurnU();

	/**
	*	V方向に反転させる
	*/
	void TurnV();

	/**
	*	画像を描画コマンドを発行する
	*/
	void Draw();

	/**
	*	U方向に反転しているかどうかを得る
	*		ture : 反転している 
	*		false : 反転していない
	*/
	bool IsTurnU() const;

	/**
	*	V方向に反転しているかどうかを得る
	*		ture : 反転している
	*		false : 反転していない
	*/
	bool IsTurnV() const;

	/**
	*	@brief	画像サイズを取得する
	*	@return	画像サイズ
	*/
	DirectX::XMFLOAT2 GetImageSize();

	/**
	*	@brief	自身のコピーを取得する
	*	@return	自身のコピー(所有権は別)
	*/
	std::shared_ptr<ImageController> GetNewCopy();

	/**
	*	@brief	使用するRootsigantureを設定する
	*	@param[in]	rootsignature	使用するrootsignatureを設定する
	*/
	void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);

	/**
	*	@brief	使用するRootsigantureを設定する
	*	@param[in]	pipelinestate	使用するpipelinestateを設定する
	*/
	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	/**
	*	@brief	使用するコマンドリストを設定する
	*	@param[in]	cmdList		使用するコマンドリスト
	*/
	void SetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);

	/**
	*	@brief	画像の名前を取得する
	*	@return	画像の名前
	*/
	std::string GetImageName() const;

	/**
	*	@brief	画像のファイルパスを取得する
	*	@return	画像のファイルパス
	*/
	std::string GetFilePath() const;
private:
	/**
	*	横幅の拡大率
	*/
	float mScaleX;

	/**
	*	縦幅の拡大率
	*/
	float mScaleY;

	/**
	*	現在の回転角度
	*/
	float mRota;

	/**
	*	画像の中心からの各頂点へのベクトルの長さ
	*/
	float mLength[4];

	/**
	*	反転しているかどうかの値
	*/
	DirectX::XMFLOAT2 mTurnSign;

	/**
	*	描画する画像の中心の描画基準点からのオフセット
	*/
	DirectX::XMFLOAT3 mCenterOffset;

	/**
	*	画像の中心からの各頂点への単位ベクトル
	*/
	DirectX::XMFLOAT3 mNormvec[4];

	/**
	*	描画の際の基準点(画面座標)回転や反転の中心
	*/
	DirectX::XMFLOAT3 mPivot;

	/**
	*	頂点情報
	*/
	ImageVertex mVertex[4];

	/**
	*	頂点情報を書き込むバッファ
	*/
	std::shared_ptr<VertexBufferObject> mVertexBuffer;

	/**
	*	画像内の切り抜きローカル座標矩形
	*/
	std::shared_ptr<Rect> mRect;

	/**
	*	このrootsigantureで使うルートパラメーター
	*/
	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE,
	};

	/**
	*	2D画像の情報
	*/
	std::shared_ptr<ImageObject> mImgObj;

	/**
	*	この画像用のDescriptorHeap
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;

	/**
	*	バンドルの更新情報をもつステート変数
	*/
	void (ImageController::*mBundleUpdate)();

	/**
	*	UV情報を更新する
	*/
	void UpdateUV();

	/**
	*	画像の中心から各頂点への単位ベクトルを更新する
	*/
	void UpdateNormvec();

	/**
	*	頂点バッファを更新する
	*/
	void UpdateBuffer();

	/**
	*	バンドルを更新する
	*/
	void UpdateBundle();

	/**
	*	バンドルを更新しない
	*/
	void NonUpdateBundle();
};

