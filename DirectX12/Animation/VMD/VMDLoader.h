#pragma once
/**
*	@addtogroup PMD_Animation
*	@file VMDLoader.h
*	@brief VMDファイルのローダー
*
*	@author 真鍋奨一郎
*	
*	@par 最終更新日	2018/9/7
*/
#include <map>
#include <string>
#include <memory>

class VMDAnimation;
class File;

/**
*	@ingroup PMD_Animation
*	@class VMDLoader
*	@brief VMDファイルをロードしデータを保持するクラス
*/
class VMDLoader
{
public:
	VMDLoader();
	~VMDLoader();

	/**
	*	@param[in]	path	VMDファイルパス
	*	@return				ロードしたVMDデータ
	*/
	std::shared_ptr<VMDAnimation> LoadAnimation(const std::string& path);
private:

	/**
	*	VMDファイルのポインタを保持する
	*/
	std::shared_ptr<File> mFp;

	/**
	*	VMDデータを保持する
	*/
	std::map<std::string, std::shared_ptr<VMDAnimation>> mAnimations;
	
	/**
	*	ロードするVMDの一時保存用
	*/
	std::shared_ptr<VMDAnimation> mLoadingAnimation;

	/**
	*	@biref	VMDのヘッダ部分をロードする
	*/
	void LoadHeader();

	/**
	*	@brief	VMDのモーションデータ部分をロードする
	*/
	void LoadAnimationDatas();

	/**
	*	@brief	VMDのモーションデータ部からポーズを計算し
	*			フレームの順番にソートする
	*/
	void CreatePoses();
};

