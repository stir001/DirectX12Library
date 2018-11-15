#pragma once
/**
*	@file Image3DController.h
*	@brief 3Dの四角形ポリゴンに2DImageを張るクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/15
*/

#include "DrawObject/DrawController3D.h"
#include "ImageControllerStructure.h"
#include <DirectXMath.h>

class VertexBufferObject;
class ConstantBufferObject;
class Rect;
class ImageObject;
class Dx12DescriptorHeapObject;

/**
*	@ingroup DrawObjectController
*	@class Image3DController
*	@brief 3D空間に存在するImageを操作するクラス
*/
class Image3DController :
	public DrawController3D
{
public:
	/**
	*	@param[in]	img		3D空間に置きたい2Dの画像情報
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	cmdList	描画コマンドを積むコマンドリスト
	*	@param[in]	piplinestate	使用するpipelinestate
	*	@param[in]	rootsignature	使用するrootsiganture
	*/
	Image3DController(std::shared_ptr<ImageObject> img,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
		std::shared_ptr<PipelineStateObject>& pipelinestate,
		std::shared_ptr<RootSignatureObject>& rootsignature);

	~Image3DController();

	/**
	*	@brief	画像を平行移動させる
	*	@param[in]	x	平行移動のx成分
	*	@param[in]	y	平行移動のy成分
	*	@param[in]	z	平行移動のz成分
	*/
	void AddCenterPos(const float x, const float y, const float z);

	/**
	*	@brief	画像を平行移動させる
	*	@param[in]	offset	平行移動成分
	*/
	void AddPosition(const DirectX::XMFLOAT3& offset);

	/**
	*	@brief	画像をすべての成分を追加で等倍に拡大縮小させる
	*	@param[in]	scale	拡大率変化量
	*/
	void AddScale(const float scale);

	/**
	*	@brief	画像を各成分ごとに追加で拡大縮小させる
	*	@param[in]	scaleX	x成分の拡大率変化量
	*	@param[in]	scaleY	y成分の拡大率変化量
	*	@param[in]	sclaeZ	z成分の拡大率変化量
	*/
	void AddScale(const float scaleX, const float scaleY, const float scaleZ);

	/**
	*	@brief	画像をx軸中心で回転させる
	*	@param[in]	deg		回転量(degree,度数法)
	*/
	void AddRotaX(float deg);

	/**
	*	@brief	画像をy軸中心で回転させる
	*	@param[in]	deg		回転量(degree,度数法)
	*/
	void AddRotaY(float deg);

	/**
	*	@brief	画像をz軸中心で回転させる
	*	@param[in]	deg		回転量(degree,度数法)
	*/
	void AddRotaZ(float deg);

	/**
	*	@brief	画像の任意の矩形部分を表示するように設定する
	*	@param[in]	inc		画像の中心を(0,0)、x軸の正の方向が右、y軸の正の方向が下の状態での矩形の中心座標
	*	@param[in]	inw		矩形の横幅
	*	@param[in]	inh		矩形の縦幅
	*/
	void SetRect(const DirectX::XMFLOAT3& inc, const float inw, const float inh);

	/**
	*	@brief	画像の任意の矩形部分を表示するように設定する
	*	@param[in]	rc		画像の中心を(0,0)、x軸の正の方向が右、y軸の正の方向が下の状態での矩形の情報
	*/
	void SetRect(const Rect& rc);

	/**
	*	@brief	任意の座標へ画像の中心を平行移動させる
	*	@param[in]	x		平行移動させるx座標
	*	@param[in]	y		平行移動させるy座標
	*	@param[in]	z		平行移動させるz座標
	*/
	void SetPosition(const float x, const float y, const float z);

	/**
	*	@brief	任意の座標へ画像の中心を平行移動させる
	*	@param[in]	setPos	平行移動させる座標
	*/
	void SetPosition(const DirectX::XMFLOAT3& setPos);

	/**
	*	@brief	画像の拡大率を等倍の倍率で設定する
	*	@param[in]	scale	設定する拡大率
	*/
	void SetScale(const float scale);

	/**
	*	@brief	画像の拡大率を各成分ごとに設定する
	*	@param[in]	scaleX	設定するX成分の拡大率
	*	@param[in]	scaleY	設定するY成分の拡大率
	*	@param[in]	scaleZ	設定するZ成分の拡大率
	*/
	void SetScale(const float scaleX, const float scaleY, const float scaleZ);

	/**
	*	@brief	U方向で画像を反転させる
	*/
	void TurnU();

	/**
	*	@brief	V方向で画像を反転させる
	*/
	void TurnV();

	/**
	*	@brief	画像を表示する
	*/
	void Draw();

	/**
	*	@brief	画像のサイズを取得する
	*	@return	画像のサイズ
	*/
	DirectX::XMFLOAT2 GetImageSize();

	/**
	*	@brief	自身のコピーを取得する
	*	@param[in]	自身のコピー	所有権は別
	*/
	std::shared_ptr<Image3DController> GetNewCopy();

	/**
	*	@brief	使用するrootsignatureを設定する
	*	@param[in]	rootsignature	使用するrootsiganture
	*/
	void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);

	/**
	*	@brief	使用するpipelinestateを設定する
	*	@param[in]	pipelinestate	使用するpipelinestate
	*/
	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	/**
	*	@brief	使用するコマンドリストを設定する
	*	@param[in]	cmdList		設定するコマンドリスト
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
	*	@enum	eROOT_PARAMATER_INDEX
	*	@brief	このクラスで使用するrootparamaterIndex
	*/
	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE,
		eROOT_PARAMATER_INDEX_CAMERA,
		eROOT_PARAMATER_INDEX_MATRIX,
	};

	/**
	*	x拡大率
	*/
	float mScaleX;

	/**
	*	y拡大率
	*/
	float mScaleY;

	/**
	*	z拡大率
	*/
	float mScaleZ;

	/**
	*	回転行列
	*/
	DirectX::XMFLOAT4X4 mRotaMatrix;

	/**
	*	アフィン行列
	*/
	DirectX::XMFLOAT4X4 mImageMatrix;

	/**
	*	反転しているかどうかの符号(1:正方向	-1:逆方向)
	*/
	DirectX::XMFLOAT2 mTurnSign;

	/**
	*	描画の際の基準点(画面座標)回転や反転の中心
	*/
	DirectX::XMFLOAT3 mCenter;

	/**
	*	法線情報
	*/
	DirectX::XMFLOAT4 mNormal;

	/**
	*	頂点情報
	*/
	Image3DVertex mVertex[4];

	/**
	*	頂点用のバッファ
	*/
	std::shared_ptr<VertexBufferObject> mVertexBuffer;

	/**
	*	アフィン行列用のバッファ
	*/
	std::shared_ptr<ConstantBufferObject> mImageMatrixBuffer;
	
	/**
	*	画像内の切り抜きローカル座標矩形
	*/
	std::shared_ptr<Rect> mRect;

	/**
	*	2D画像情報を持つ
	*/
	std::shared_ptr<ImageObject> mImgObj;

	/**
	*	DescriptorHeap
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;

	/**
	*	bundle用のアップデートステート変数
	*/
	void (Image3DController::*mBundleUpdate)();

	/**
	*	UVを更新する
	*/
	void UpdateUV();

	/**
	*	アフィン行列を更新する
	*/
	void UpdateMatrix();

	/**
	*	アフィン行列をバッファに書き込む
	*/
	void UpdateBuffer();

	/**
	*	バンドルを更新する
	*/
	void UpdateBundle();

	/**
	*	バンドルの更新をしない
	*/
	void NonUpdateBundle();
};

