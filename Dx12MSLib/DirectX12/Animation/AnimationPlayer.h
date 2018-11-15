#pragma once
/**
*	@addtogroup AnimationPlayer
*	@file AnimationPlayer.h
*	@brief �A�j���[�V�����X�V�N���X�̊��N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	�X�V��
*/

class AnimationPlayerManager;

/**
*	@ingroup AnimationPlayer
*	@class AnimationPlayer
*	@brief �A�j���[�V�����X�V�N���X�̊��N���X
*/
class AnimationPlayer
{
	friend AnimationPlayerManager;
public:
	AnimationPlayer();
	virtual ~AnimationPlayer();

	/**
	*	�A�j���[�V�����̌v�Z����������
	*/
	virtual void Update() = 0;

	/**
	*	AnimationPlayerManager���犄�蓖�Ă���ID���擾����
	*/
	virtual int GetID() const;

	/**
	*	���݂̃t���[���Œ�~���Ă��邩�ǂ����𔻒肷��
	*/
	virtual bool IsEnd() const;
protected:

	/**
	*	����U��ꂽID �o�^����Ă��Ȃ��ꍇ��-1
	*/
	int mAnimationId;

	/**
	*	���[�v���邩�ǂ����̃t���O true:���[�v���� false:���[�v���Ȃ�
	*/
	bool mIsLoop;

	/**
	*	�I�����Ă��邩�ǂ����̃t���O true:�I�����Ă��� false:�I�����Ă��Ȃ�
	*/
	bool mIsEnd;
};

