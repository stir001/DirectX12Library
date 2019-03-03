#pragma once
/**
*	@file AnimationPlayerManager
*	@brief AnimationPlayerManager.h
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/10
*/
#include <map>
#include <thread>
#include <future>

class AnimationPlayer;

/**
*	@ingroup Animation
*	@class AnimationPlayerManager
*	@brief アニメーションの更新を管理するクラス
*			毎フレーム登録されているアニメーションの再生を
*			別スレッドに投げて計算する
*/
class AnimationPlayerUpdater
{
public:
	~AnimationPlayerUpdater();

	/**
	*	登録されているアニメーションの計算を別スレッドに投げる
	*/
	void UpdateAnimations();

	/**
	*	インスタンスを取得
	*/
	static AnimationPlayerUpdater& Instance();

	/**
	*	@brief	再生するアニメーションを登録する
				登録されたアニメーションは自動的に再生される
	*	@param[in]	anim	登録するアニメーション
	*/
	int SetAnimation(AnimationPlayer* anim);

	/**
	*	@brief	対象のアニメーションの再生を止める
	*	@param[in]	id	登録されている計算を止めたい対象のアニメーションID
	*/
	void StopAnimation(int id);

	/**
	*	@brief	アニメーションの計算を再開する
	*	@param[in]	id	登録されている計算を再開したい対象のアニメーションID
	*/
	void ReStartAnimation(int id);

	/**
	*	@brief	アニメーションを登録から外す
	*	@param[i]	id	登録されている登録から外したい対象のアニメーションID
	*	@return		-1	無効なアニメーションIDを返す
	*/
	int RemoveAnimation(int id);

	/**
	*	アニメーションの計算が終了するまで待機する
	*/
	void WaitAnimation();

	/**
	*	すべてのアニメーションの計算を停止する
	*/
	void StopAllAnimation();

	/**
	*	すべてのアニメーションの更新を再開する
	*/
	void ReStartAllAnimation();

	/**
	*	@struct AnimationInfo
	*	@brief アニメーションの情報を持つ
	*/
	struct AnimatiomInfo
	{
		/**
		*	登録されているアニメーション
		*/
		AnimationPlayer* mAnimPlayer;

		/**
		*	割り振られているアニメーションのID
		*/
		unsigned int id;

		/**
		*	現在再生中かどうかのフラグ	true:再生している false:再生していない
		*/
		bool isPaly;
	};

private:
	AnimationPlayerUpdater();

	/**
	*	別スレッドに計算を投げるためのスレッドオブジェクト
	*/
	std::thread mThreadObject;

	/**
	*	アニメーション情報を保持する
	*/
	std::map<int,AnimatiomInfo> mAnimations;

	/**
	*	登録されているアニメーションを計算するステートを保持する
	*/
	void (AnimationPlayerUpdater::*mUpdate)();

	/**
	*	@brief	指定されているIDを保持しているAnimationInfoのイテレータを返す
	*	@param[in]	id	探すアニメーションID
	*	@return		対象のアニメーションイテレータ	ない場合はend()を返す
	*/
	std::map<int,AnimatiomInfo>::iterator FindAnimItr(int id);

	/**
	*	@brief	未登録で最も値の小さいIDを取得する
	*	@return	未登録ID
	*/
	int FindMostSmallId();

	/**
	*	アニメーションの更新をしないステート
	*/
	void NonUpdate();

	/**
	*	通常の更新をするステート
	*/
	void CommonUpdate();

	/**
	*	スレッドが終わっているか否かを持つフラグ ture:終わっていない false:終わっている
	*/
	bool mIsThreadRun;
};

