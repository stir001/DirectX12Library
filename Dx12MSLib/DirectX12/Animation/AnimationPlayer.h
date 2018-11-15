#pragma once
/**
*	@addtogroup AnimationPlayer
*	@file AnimationPlayer.h
*	@brief アニメーション更新クラスの基底クラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	更新日
*/

class AnimationPlayerManager;

/**
*	@ingroup AnimationPlayer
*	@class AnimationPlayer
*	@brief アニメーション更新クラスの基底クラス
*/
class AnimationPlayer
{
	friend AnimationPlayerManager;
public:
	AnimationPlayer();
	virtual ~AnimationPlayer();

	/**
	*	アニメーションの計算処理をする
	*/
	virtual void Update() = 0;

	/**
	*	AnimationPlayerManagerから割り当てられるIDを取得する
	*/
	virtual int GetID() const;

	/**
	*	現在のフレームで停止しているかどうかを判定する
	*/
	virtual bool IsEnd() const;
protected:

	/**
	*	割り振られたID 登録されていない場合は-1
	*/
	int mAnimationId;

	/**
	*	ループするかどうかのフラグ true:ループする false:ループしない
	*/
	bool mIsLoop;

	/**
	*	終了しているかどうかのフラグ true:終了している false:終了していない
	*/
	bool mIsEnd;
};

