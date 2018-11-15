#pragma once
/**
*	@file VMDPlayer
*	@brief VMDのモーションデータ再生クラスの定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/7
*/

#include "Animation/AnimationPlayer.h"

#include <memory>
#include <DirectXMath.h>
#include <vector>
#include <map>
#include <string>

class VMDMotion;
class ConstantBufferObject;
struct VMDPose;
struct PMDBoneData;
struct BoneTree;

/**
*	@ingroup PMD_Animation
*	@class VMDPlayer
*	@brief VMDのモーションをフレームを基準に計算する
*/
class VMDPlayer : public AnimationPlayer
{
public:
	VMDPlayer(std::vector<PMDBoneData>& bDatas, BoneTree& node, std::vector<DirectX::XMFLOAT4X4>& boneMat, std::shared_ptr<ConstantBufferObject>& boneConstantBuffer);
	~VMDPlayer();

	/**
	*	計算を止める
	*/
	void Stop();

	/**
	*	@brief 計算を始める
	*	@param[in]	isLoop	ループ処理をするかどうかのフラグ
	*						true:ループ処理をする
	*						false:ループ処理をしない
	*/
	void Play(bool isLoop = false);

	/**
	*	状態に応じて計算をする
	*/
	void Update();

	/**
	*	@brief コンスタントバッファにボーンマトリックスを書き込む
	*	@param[in,out]	matrixBuffer	書き込むコンスタントバッファ
	*/
	void WriteBoneMatrix(std::shared_ptr<ConstantBufferObject>& matrixBuffer);

	/**
	*	@brief 計算するVMDMotionを設定する
	*	@param[in]	vmd		計算するVMDMotion
	*/
	void SetVMD(std::shared_ptr<VMDMotion>& vmd);
private:
	/**
	*	VMDPlayerを保持しているモデルのボーン回転情報
	*/
	std::vector<DirectX::XMFLOAT4X4>& mCurrentBoneMatrix;

	/**
	*	ボーンの回転情報を書き込むバッファ
	*/
	std::shared_ptr<ConstantBufferObject>& mBoneConstantBuffer;

	/**
	*	ポーズ情報へのポインタ
	*/
	std::map<std::string, std::vector<VMDPose>>* mPoses;

	/**
	*	ボーンの情報
	*/
	std::vector<PMDBoneData>& mBoneDatas;

	/**
	*	ボーンの親子関係情報
	*/
	BoneTree& mBoneNode;

	/**
	*	現在のフレームナンバー
	*/
	int mFrame;

	/**
	*	現在のアニメーションの終了フレーム
	*/
	int mMaxFrame;

	/**
	*	アニメーションをループするかどうかのフラグ
	*	ture:ループする	false:ループしない
	*/
	bool mIsLoop;

	/**
	*	更新ステート保持変数
	*/
	void (VMDPlayer::*mUpdate)();

	/**
	*	終了チェックステート保持変数
	*/
	void (VMDPlayer::*mEndCheck)();

	/**
	*	@brief	対象のボーンを回転させる
	*	@param[in]	boneName	回転させるボーンの名前
	*	@param[in]	boneRotaMatrix	ボーン回転行列
	*/
	void VMDBoneRotation(const std::string& boneName, const DirectX::XMMATRIX& boneRotaMatrix);

	/**
	*	@brief	再帰的に対象のボーンの子ボーンを回転させる
	*	@param[i]	parentBoneMatrix	ボーン回転行列
	*	@param[i]	子供を回転させたいボーンのインデックス
	*/
	void VMDBoneChildRotation(const DirectX::XMFLOAT4X4& parentBoneMatrix, int parentIndex);

	/**
	*	終了チェックをするステート
	*/
	void EndCheck();

	/**
	*	終了チェックをしないステート
	*/
	void NonCheck();

	/**
	*	更新状態のUpdateステート
	*/
	void PlayingUpdate();

	/**
	*	停止状態のUpdateステート
	*/
	void StopUpdate();
};

