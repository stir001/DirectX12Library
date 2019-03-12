#pragma once
/**
*	@file ImageController.h
*	@brief 2D画像の操作を行うクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/21
*/
#include "DirectX12/DrawObject/DrawObjectController.h"
#include "ImageControllerStructure.h"

#include <memory>
#include <DirectXMath.h>

class ImageObject;
class Rect;
class VertexBufferObject;
class Dx12DescriptorHeapObject;
class Dx12CommandList;
class ImageDrawIssuer;

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
		std::shared_ptr<Dx12CommandList> uicmdList,
		std::shared_ptr<Dx12CommandList> backcmdList,
		std::shared_ptr<PipelineStateObject>& pipelinestate,
		std::shared_ptr<RootSignatureObject>& rootsignature);
	~ImageController();
	/**
	*	@brief	画像を平行移動させる 画面中央原点 +x右 +y上
	*	@param[in]	x	xの平行移動成分
	*	@param[in]	y	yの平行移動成分
	*	@param[in]	z	zの平行移動成分
	*/
	void AddPos(const float x, const float y, const float z);

	/**
	*	@brief	画像を平行移動させる 画面中央原点 +x右 +y上
	*	@param[in]	offset	平行移動線分
	*/
	void AddPos(const DirectX::XMFLOAT3& offset);

	/**
	*	@brief	画像の拡大率を変化させる
	*	@param[in]	scale	画像の拡大率の変化量
	*/
	void AddScale(const float scale);

	/**
	*	@brief	画像を回転させる 正の値で反時計回り
	*	@param[in]	rad		回転量(radian,弧度法)
	*/
	void AddRota(const float rad);

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
	*	@brief	画像の回転を設定する  正の値で反時計回り
	*	@param[in]	rad		設定したい回転角度(radian,弧度法)
	*/
	void SetRota(const float rad);

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
	*	画像をUIレイヤーに描画する
	*/
	void Draw();

	/**
	*	画像をBackGroundレイヤーに描画する
	*/
	void BackDraw();

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
	std::shared_ptr<ImageController> Duplicate();

	/**
	*	@brief	使用するRootsigantureを設定する
	*	@param[in]	rootsignature	使用するrootsignatureを設定する
	*/
	void SetGraphicsRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);

	/**
	*	@brief	使用するRootsigantureを設定する
	*	@param[in]	pipelinestate	使用するpipelinestateを設定する
	*/
	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

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
	*	拡大率
	*/
	DirectX::XMFLOAT2 mScale;

	/**
	*	現在の回転角度
	*/
	float mRota;

	/**
	*	反転しているかどうかの値
	*/
	DirectX::XMFLOAT2 mTurnSign;

	/**
	*	描画する画像の中心の描画基準点からのオフセット
	*/
	DirectX::XMFLOAT3 mCenterOffset;

	/**
	*	描画の際の基準点(画面座標)回転や反転の中心
	*/
	DirectX::XMFLOAT3 mPivot;

	/**
	*	変換行列
	*/
	std::vector<DirectX::XMFLOAT4X4> mInstanceMat;

	/**
	*	変換行列計算用行列
	*/
	DirectX::XMFLOAT4X4 mCalMatrix;

	/**
	*	UV計算用
	*/
	ImageUVSet mCalUV;

	/**
	*	頂点情報
	*/
	ImageVertex mVertex[4];

	/**
	*	UV情報
	*/
	std::vector<ImageUVSet> mUVs;

	/**
	*	頂点情報を書き込むバッファ
	*/
	std::shared_ptr<VertexBufferObject> mVertexBuffer;

	/**
	*	UV情報を書き込むバッファ
	*/
	std::shared_ptr<VertexBufferObject> mUVBuffer;

	/**
	*	変換行列情報を書き込むバッファ
	*/
	std::shared_ptr<VertexBufferObject> mMatrixBuffer;

	/**
	*	画像内の切り抜きローカル座標矩形
	*/
	std::shared_ptr<Rect> mRect;

	/**
	*	2D画像の情報
	*/
	std::shared_ptr<ImageObject> mImgObj;

	/**
	*	この画像用のDescriptorHeap
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;

	/**
	*	背景描画用のコマンドリスト
	*/
	std::shared_ptr<Dx12CommandList> mBackCmdList;

	/**
	*	バンドルの更新情報をもつステート変数
	*/
	void (ImageController::*mBundleUpdate)();

	/**
	*	描画命令発行用クラス
	*/
	std::shared_ptr<ImageDrawIssuer> mDrawIssuer;

	/**
	*	UV情報を更新する
	*/
	void UpdateUV();

	/**
	*	頂点バッファを更新する
	*/
	void UpdateInstanceMatrix();

	/**
	*	バンドルを更新する
	*/
	void UpdateBundle();

	/**
	*	バンドルを更新しない
	*/
	void NonUpdateBundle();

	/**
	*	描画命令発行クラスを生成する
	*/
	void CreateDrawIssuer();

	/**
	*	変換行列の更新
	*/
	void WriteMatrixBuffer();

	/**
	*	UVバッファを更新
	*/
	void WriteUVBuffer();

	/**
	*	インスタンシング用のデータを作成する
	*/
	void CreateInstanceData();

	/**
	*	現在計算しているインスタンスのIDを取得する
	*/
	unsigned int GetCurrentInstanceID() const;

	/**
	*	同じ名前でVertexBufferを再生成する
	*/
	void RemakeVertexBuffer(std::shared_ptr<VertexBufferObject>& buffer, unsigned int elementSize, unsigned int elementNum);
};

