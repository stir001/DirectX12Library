#pragma once
/**
*	@file VMDAnimation.h
*	@brief VMDAnimationの定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/07
*/

#include "DirectX12/Animation/Animation.h"

#include <DirectXMath.h>
#include <map>
#include <vector>
#include <string>

class PMDModel;

/**
*	@ingroup PMD_Animation
*	@ingroup Animation
*	@struct VMDHeader
*	@brief VMDのヘッダ部分
*/
struct VMDHeader
{
	/**
	*	ヘッダ部分
	*/
	unsigned char header[30];

	/**
	*	対象のモデルの名前
	*/
	unsigned char modelName[20];

	/**
	*	キーフレームの情報が何個あるか
	*/
	unsigned int count;
};

/**
*	@ingroup PMD_Animation
*	@struct VMDAnimationData
*	@brief モーションデータの詳細
*/
struct VMDAnimationData
{
	/**
	*	ボーンの名前
	*/
	char boneName[15];

	/**
	*	フレームの番号
	*/
	unsigned long frameNo;

	/**
	*	座標情報
	*/
	DirectX::XMFLOAT3 location;

	/**
	*	回転情報
	*/
	DirectX::XMFLOAT4 quoternion;//回転

	/**
	*	補間パラメータ
	*/
	unsigned char interpolation[64];
};

/**
*	@ingroup PMD_Animation
*	@struct	VMDPose
*	@brief ポーズのフレームと回転情報
*/
struct VMDPose
{
	/**
	*	フレーム番号
	*/
	int frameNo;

	/**
	*	回転情報
	*/
	DirectX::XMFLOAT4 quoternion;

	DirectX::XMFLOAT3 location;
};

/**
*	@ingroup PMD_Animation
*	@class VMDAnimation
*	@brief VMDのモーション詳細を保持する
*/
class VMDAnimation :public Animation
{
	friend class VMDLoader;
	friend class VMDPlayer;
public:
	VMDAnimation();
	~VMDAnimation();
private:
	/**
	*	ヘッダ情報を保持
	*/
	VMDHeader mHeader;

	/**
	*	モーションデータを保持
	*/
	std::vector<VMDAnimationData> mAnimations;

	/**
	*	ボーンの名前ごとにポーズを保持
	*/
	std::map<std::string, std::vector<VMDPose>> mPoses;
};

