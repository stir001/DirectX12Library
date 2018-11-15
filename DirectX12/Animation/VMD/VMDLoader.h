#pragma once
/**
*	@addtogroup PMD_Animation
*	@file VMDLoader.h
*	@brief VMD�t�@�C���̃��[�_�[
*
*	@author �^�珧��Y
*	
*	@par �ŏI�X�V��	2018/9/7
*/
#include <map>
#include <string>
#include <memory>

class VMDMotion;
class File;

/**
*	@ingroup PMD_Animation
*	@class VMDLoader
*	@brief VMD�t�@�C�������[�h���f�[�^��ێ�����N���X
*/
class VMDLoader
{
public:
	VMDLoader();
	~VMDLoader();

	/**
	*	@param[in]	path	VMD�t�@�C���p�X
	*	@return				���[�h����VMD�f�[�^
	*/
	std::shared_ptr<VMDMotion> LoadMotion(const std::string& path);
private:

	/**
	*	VMD�t�@�C���̃|�C���^��ێ�����
	*/
	std::shared_ptr<File> mFp;

	/**
	*	VMD�f�[�^��ێ�����
	*/
	std::map<std::string, std::shared_ptr<VMDMotion>> mMotions;
	
	/**
	*	���[�h����VMD�̈ꎞ�ۑ��p
	*/
	std::shared_ptr<VMDMotion> mLoadingMotion;

	/**
	*	@biref	VMD�̃w�b�_���������[�h����
	*/
	void LoadHeader();

	/**
	*	@brief	VMD�̃��[�V�����f�[�^���������[�h����
	*/
	void LoadMotionDatas();

	/**
	*	@brief	VMD�̃��[�V�����f�[�^������|�[�Y���v�Z��
	*			�t���[���̏��ԂɃ\�[�g����
	*/
	void CreatePoses();
};

