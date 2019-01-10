#pragma once
/**
*	@addtogroup Dx12Camera
*	@file Dx12Camera.h
*	@brief DirectX12で使用するカメラクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/23
*/
#include "CameraBufferStructure.h"

#include <DirectXMath.h>
#include <wrl.h	>
#include <memory>
#include <d3d12.h>

class ConstantBufferObject;
class DxInput;
class CameraHolder;
struct ID3D12GraphicsCommandList;

/**
*	@ingroup Dx12Camera
*	@class Dx12Camera
*	@brief DirectX12で使うための機能を持つカメラクラス
*/
class Dx12Camera : public std::enable_shared_from_this<Dx12Camera>
{
public:
	/**
	*	@param[in]	width	カメラの映すピクセル単位の横幅
	*	@param[in]	height	カメラの映すピクセル単位の縦幅
	*/
	Dx12Camera(int wWidth, int wHeight);

	/**
	*	@param[in]	width	カメラの映すピクセル単位の横幅
	*	@param[in]	height	カメラの映すピクセル単位の縦幅
	*	@param[in]	eye		カメラの位置
	*	@param[in]	target	カメラの注視点
	*	@param[in]	upper	カメラの上ベクトル(world座標基準)
	*/
	Dx12Camera(int wWidth, int wHeight, const DirectX::XMFLOAT3& eye,
		const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& upper);

	/**
	*	@param[in]	width	カメラの映すピクセル単位の横幅
	*	@param[in]	height	カメラの映すピクセル単位の縦幅
	*	@param[in]	eye		カメラの位置
	*	@param[in]	target	カメラの注視点
	*	@param[in]	upper	カメラの上ベクトル(world座標基準)
	*/
	Dx12Camera(D3D12_VIEWPORT viewport, D3D12_RECT scissorRect, const DirectX::XMFLOAT3& eye,
		const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& upper,
		std::shared_ptr<CameraHolder> holder, unsigned int holdIndex);
	~Dx12Camera();

	/**
	*	@brief	カメラの座標を設定する
	*	@param[in]	pos		設定する座標
	*/
	void SetPos(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	カメラの座標を設定する
	*	@param[in]	x	設定するx座標
	*	@param[in]	y	設定するy座標
	*	@param[in]	z	設定するz座標
	*/
	void SetPos(float x, float y, float z);

	/**
	*	@brief	カメラの注視点を設定する
	*	@param[in]	target	設定する座標
	*/
	void SetTarget(const DirectX::XMFLOAT3& target);

	/**
	*	@brief	カメラの注視点を設定する
	*	@param[in]	x	設定する注視点x座標
	*	@param[in]	y	設定する注視点y座標
	*	@param[in]	z	設定する注視点z座標
	*/
	void SetTarget(float x, float y, float z);

	/**
	*	@brief	視線の方向を変えないように
				カメラの座標を設定する(注視点も一緒にずれる)
	*	@param[in]	pos		設定する座標
	*/
	void ParallelSetPos(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	ワールド基準でX軸回転する
	*	@param[in]	deg		回転量(degree,度数法)
	*/
	void AddXAxisRota(float deg);

	/**
	*	@brief	ワールド基準でY軸回転する
	*	@param[in]	deg		回転量(degree,度数法)
	*/
	void AddYAxisRota(float deg);

	/**
	*	@brief	ワールド基準でZ軸回転する
	*	@param[in]	deg		回転量(degree,度数法)
	*/
	void AddZAxisRota(float deg);

	/**
	*	@brief	y方向へ移動する
	*	@param[in]	val	移動成分の大きさ
	*/
	void MoveUp(float val);

	/**
	*	@brief	カメラのローカル前方向へ移動する
	*	@param[in]	val	移動成分の大きさ
	*/
	void MoveFront(float vel);

	/**
	*	@brief	XZ平面上のローカル横方向へ移動する
	*	@param[in]	val	移動成分の大きさ
	*/
	void MoveSide(float vel);

	/**
	*	@brief	Y軸中心で回転する
	*	@param[in]	deg	回転成分の大きさ(degree,度数法)
	*/
	void TurnRightLeft(float deg);

	/**
	*	@brief	カメラのローカル上方向へ回転する
	*	@param[in]	deg	回転成分の大きさ(degree,度数法)
	*/
	void TurnUpDown(float deg);

	/**
	*	@brief ViewPortを設定する 描画する画面内の大きさ
	*	@param width	描画範囲の横幅
	*	@param height	描画範囲の縦幅
	*	@param topLX	描画範囲の左上X座標
	*	@param topLY	描画範囲の左上Y座標
	*	@param minDepth	深度の最小値
	*	@param maxDepth	深度の最大値
	*/
	void SetViewPort(float width, float height,
		float topLX = 0.0f, float topLY = 0.0f,
		float minDepth = 0.0f, float maxDepth = 1.0f);

	/**
	*	@brief	viewporの値を画面比で決定する 左上(0,0) 右下(1,1)
	*	@param[in]	left		左端x画面座標比
	*	@param[in]	top			上端y画面座標比
	*	@param[in]	right		右端x画面座標比
	*	@param[in]	bottom		下端y画面座標比
	*	@param[in]	minDepth	最小depth値
	*	@param[in]	maxDepth	最大depth値
	*/
	void SetViewPortNormalizeValue(float left, float top
		, float right, float bottom
		, float minDepth = 0.0f, float maxDepth = 1.0f);

	/**
	*	@brief ScisorRectを設定する ViewPort内の切り抜き矩形(画面座標)
	*	@param right	切り抜き矩形の右のX座標
	*	@param bottom	切り抜き矩形の下Y座標
	*	@param left		切り抜き矩形の左X座標
	*	@param top		切り抜き矩形の上Y座標
	*/
	void SetScisorRect(int right, int bottom,
		int left = 0.0f, int top = 0.0f );

	/**
	*	@brief ScisorRectを画面比で決定する 
	*		0~横幅 : 0 ~ 1.0, 0~縦幅 : 0~1.0
	*	@param[in]	left	左端の画面比(0~1.0)
	*	@param[in]	top		上端の画面比(0~1.0)
	*	@param[in]	right	右端の画面比(0~1.0)
	*	@param[in]	bottom	下端の画面比(0~1.0)
	*/
	void SetScisorRectNormalizeValue(float left, float top
		,float right, float bottom);

	/**
	*	カメラのViewPortを取得する
	*/
	D3D12_VIEWPORT GetViewPort() const;

	/**
	*	カメラのScisorRectを取得する
	*/
	D3D12_RECT GetScissorRect() const;

	/**
	*	@brief	あらかじめ提供されている移動でカメラを移動させる
	*	@param[in]	input	現在の入力情報
	*/
	void DefaultMove(const DxInput& input);

	/**
	*	@brief	カメラのView行列を得る
	*	@return	カメラのView行列
	*/
	DirectX::XMFLOAT4X4 GetView();

	/**
	*	@brief	カメラのプロジェクション行列を得る
	*	@return	カメラのプロジェクション行列
	*/
	DirectX::XMFLOAT4X4 GetProjection();

	/**
	*	@brief	カメラの座標を得る
	*	@return	カメラの座標
	*/
	DirectX::XMFLOAT4 GetCameraPosition();

	/**
	*	@brief	カメラのワールド行列を得る
	*	@return	カメラのワールド行列
	*/
	DirectX::XMFLOAT4X4 GetWorld();

	/**
	*	カメラ情報を取得する
	*/
	Dx12CameraBuffer GetCameraBufferElement();

	/**
	*	カメラが保持されているインデックス
	*/
	int GetHoldIndex() const;

	/**
	*	@brief fovを設定する
	*	@param[in]	rad		視野角(ラジアン)
	*/
	void SetFoV(float rad);

	/**
	*	@brief	クリッピングのnearを
	*	@param[in]	cameraNear	ニアの距離
	*/
	void SetNear(float cameraNear);

	/**
	*	@brief	カメラの
	*/
	void SetFar(float cameraFar);

	/**
	*	画面の大きさに合わせてviewportとscissorRectを更新する(画面比ベース)
	*/
	void UpdateViewportScisoorRect();
	/**
	*	@brief	カメラが対応しているviewportのサイズを取得する
	*/
	DirectX::XMINT2 GetViewPortSize() const;

	/**
	*	@brief	fovを取得する
	*	@return	現在のfov
	*/
	float GetFov() const;
private:
	/**
	*	@struct	ViewportやScissorRectの画面比保存用構造体
	*/
	struct RectRatio
	{
		float left;		//!	左端x画面座標比率
		float top;		//!	上端y画面座標比率
		float right;	//!	右端x画面座標比率
		float bottom;	//!	下端y画面座標比率
		RectRatio() :left(0.0f), top(0.0f), right(1.0f), bottom(1.0f) {}
		RectRatio(float left, float top, float right, float bottom)
			: left(left), top(top), right(right), bottom(bottom) {}
	};

	/**
	*	カメラがGPUに投げる要素
	*/
	Dx12CameraBuffer mElement;

	/**
	*	カメラのView行列
	*/
	DirectX::XMFLOAT4X4 mCamera;

	/**
	*	カメラのProjection行列
	*/
	DirectX::XMFLOAT4X4 mProjection;

	/**
	*	カメラの上ベクトル
	*/
	DirectX::XMFLOAT3 mUpper;

	/**
	*	カメラのワールド行列
	*/
	DirectX::XMFLOAT4X4 mWorldRotation;

	/**
	*	カメラのローカル上ベクトル
	*/
	DirectX::XMFLOAT3 mLocalUpper;

	/**
	*	カメラの映すピクセル単位の横幅
	*/
	int mWidth;

	/**
	*	カメラの映すピクセル単位の縦幅
	*/
	int mHeight;

	/**
	*	ConstantBufferのbyte単位のサイズ
	*/
	int mBuffersize;

	/**
	*	画面の描画範囲矩形
	*/
	D3D12_VIEWPORT mViewPort;

	/**
	*	viewPortの画面比率
	*/
	RectRatio mViewPortRatio;

	/**
	*	描画範囲内の切り抜き矩形
	*/
	D3D12_RECT mScissorRect;

	/**
	*	sciddorRectの画面比率保存用
	*/
	RectRatio mScissorRectRatio;

	/**
	*	自分を保持しているホルダー
	*/
	std::weak_ptr<CameraHolder> mHolder;

	/**
	*	自分の管理されているインデックス
	*/
	int mHoldIndex;

	/**
	*	視野角
	*/
	float mFov;

	/**
	*	nearClipping
	*/
	float mNear;

	/**
	*	farClipping
	*/
	float mFar;

	/**
	*	更新をCameraHolderに通知するステート用
	*/
	void (Dx12Camera::*mHolderSetter)();

	/**
	*	@brief	カメラに回転行列を適応する
	*	@param[in]	rotaMatrix	回転行列
	*/
	void AddRotationAxis(const DirectX::XMMATRIX& rotaMatrix);

	/**
	*	@brief	ローカル上ベクトルを得る
	*	@return	ローカル上ベクトル
	*/
	DirectX::XMFLOAT3 GetLocalUpper();

	/**
	*	@brief	視線ベクトルを得る
	*	@return	視線ベクトル
	*/
	DirectX::XMFLOAT3 GetEyeToTargetVec();

	/**
	*	シェーダーに渡す要素を更新する
	*/
	void UpdateElement();

	/**
	*	初期化処理
	*/
	void Init();

	/**
	*	holderにカメラ情報変更後の値を設定する状態
	*/
	void SetElementToHolder();

	/**
	*	holderにカメラ情報変更後の値を設定しない状態
	*/
	void NonSetElementToHolder();

	/**
	*	projection行列を更新する
	*/
	void UpdateProjection();

	/**
	*	viewportとsissorRectの値と画面サイズを参考に比率を更新する
	*/
	void UpdateRatios();
};
