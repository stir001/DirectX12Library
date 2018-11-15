#pragma once
/**
*	@file AnimationPlayerManager
*	@brief AnimationPlayerManager.h
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/10
*/
#include <list>
#include <thread>
#include <future>

class AnimationPlayer;

/**
*	@ingroup Animation
*	@class AnimationPlayerManager
*	@brief �A�j���[�V�����̍X�V���Ǘ�����N���X
*			���t���[���o�^����Ă���A�j���[�V�����̍Đ���
*			�ʃX���b�h�ɓ����Čv�Z����
*/
class AnimationPlayerManager
{
public:
	~AnimationPlayerManager();

	/**
	*	�o�^����Ă���A�j���[�V�����̌v�Z��ʃX���b�h�ɓ�����
	*/
	void UpdateAnimations();

	/**
	*	�C���X�^���X���擾
	*/
	static AnimationPlayerManager& Instance();

	/**
	*	@brief	�Đ�����A�j���[�V������o�^����
				�o�^���ꂽ�A�j���[�V�����͎����I�ɍĐ������
	*	@param[in]	anim	�o�^����A�j���[�V����
	*/
	int SetAnimation(AnimationPlayer* anim);

	/**
	*	@brief	�Ώۂ̃A�j���[�V�����̍Đ����~�߂�
	*	@param[in]	id	�o�^����Ă���v�Z���~�߂����Ώۂ̃A�j���[�V����ID
	*/
	void StopAnimation(int id);

	/**
	*	@brief	�A�j���[�V�����̌v�Z���ĊJ����
	*	@param[in]	id	�o�^����Ă���v�Z���ĊJ�������Ώۂ̃A�j���[�V����ID
	*/
	void ReStartAnimation(int id);

	/**
	*	@brief	�A�j���[�V������o�^����O��
	*	@param[i]	id	�o�^����Ă���o�^����O�������Ώۂ̃A�j���[�V����ID
	*	@return		-1	�����ȃA�j���[�V����ID��Ԃ�
	*/
	int RemoveAnimation(int id);

	/**
	*	�A�j���[�V�����̌v�Z���I������܂őҋ@����
	*/
	void WaitSafeFree();

	/**
	*	���ׂẴA�j���[�V�����̌v�Z���~����
	*/
	void StopAllAnimation();

	/**
	*	���ׂẴA�j���[�V�����̍X�V���ĊJ����
	*/
	void ReStartAllAnimation();

	/**
	*	@struct AnimationInfo
	*	@brief �A�j���[�V�����̏�������
	*/
	struct AnimatiomInfo
	{
		/**
		*	�o�^����Ă���A�j���[�V����
		*/
		AnimationPlayer* mAnimPlayer;

		/**
		*	����U���Ă���A�j���[�V������ID
		*/
		unsigned int id;

		/**
		*	���ݍĐ������ǂ����̃t���O	true:�Đ����Ă��� false:�Đ����Ă��Ȃ�
		*/
		bool isPaly;
	};

private:
	AnimationPlayerManager();

	/**
	*	�ʃX���b�h�Ɍv�Z�𓊂��邽�߂̃X���b�h�I�u�W�F�N�g
	*/
	std::thread mThreadObject;

	/**
	*	�A�j���[�V��������ێ�����
	*/
	std::list<AnimatiomInfo> mAnimations;

	/**
	*	�o�^����Ă���A�j���[�V�������v�Z����X�e�[�g��ێ�����
	*/
	void (AnimationPlayerManager::*mUpdate)();

	/**
	*	@brief	�w�肳��Ă���ID��ێ����Ă���AnimationInfo�̃C�e���[�^��Ԃ�
	*	@param[in]	id	�T���A�j���[�V����ID
	*	@return		�Ώۂ̃A�j���[�V�����C�e���[�^	�Ȃ��ꍇ��end()��Ԃ�
	*/
	std::list<AnimatiomInfo>::iterator FindAnimItr(int id);

	/**
	*	@brief	���o�^�ōł��l�̏�����ID���擾����
	*	@return	���o�^ID
	*/
	int FindMostSmallId();

	/**
	*	�A�j���[�V�����̍X�V�����Ȃ��X�e�[�g
	*/
	void NonUpdate();

	/**
	*	�ʏ�̍X�V������X�e�[�g
	*/
	void CommonUpdate();

	/**
	*	�X���b�h���I����Ă��邩�ۂ������t���O ture:�I����Ă��Ȃ� false:�I����Ă���
	*/
	bool mIsThreadRun;
};

