#pragma once
/**
*	@file VMDPlayer
*	@brief VMD�̃��[�V�����f�[�^�Đ��N���X�̒�`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/7
*/

#include "DirectX12/Animation/AnimationPlayer.h"

#include <memory>
#include <DirectXMath.h>
#include <vector>
#include <map>
#include <string>
#include <list>

class VMDAnimation;
class ConstantBufferObject;
struct VMDPose;
struct PMDBoneData;
struct BoneTree;

/**
*	@ingroup PMD_Animation
*	@class VMDPlayer
*	@brief VMD�̃��[�V�������t���[������Ɍv�Z����
*/
class VMDPlayer : public AnimationPlayer
{
public:
	VMDPlayer(std::vector<PMDBoneData>& bDatas, BoneTree& node, std::vector<DirectX::XMFLOAT4X4>& boneMat, std::shared_ptr<ConstantBufferObject>& boneConstantBuffer);
	~VMDPlayer();

	/**
	*	�v�Z���~�߂�
	*/
	void Stop();

	/**
	*	@brief �v�Z���n�߂�
	*	@param[in]	isLoop	���[�v���������邩�ǂ����̃t���O
	*						true:���[�v����������
	*						false:���[�v���������Ȃ�
	*/
	void Play(bool isLoop = false);

	/**
	*	��Ԃɉ����Čv�Z������
	*/
	void Update();

	/**
	*	@brief �R���X�^���g�o�b�t�@�Ƀ{�[���}�g���b�N�X����������
	*	@param[in,out]	matrixBuffer	�������ރR���X�^���g�o�b�t�@
	*/
	void WriteBoneMatrix(std::shared_ptr<ConstantBufferObject>& matrixBuffer);

	/**
	*	@brief �v�Z����VMDAnimation��ݒ肷��
	*	@param[in]	vmd		�v�Z����VMDAnimation
	*/
	void SetVMD(std::shared_ptr<VMDAnimation>& vmd);
private:
	/**
	*	VMDPlayer��ێ����Ă��郂�f���̃{�[����]���
	*/
	std::vector<DirectX::XMFLOAT4X4>& mCurrentBoneMatrix;

	/**
	*	�{�[���̉�]�����������ރo�b�t�@
	*/
	std::shared_ptr<ConstantBufferObject>& mBoneConstantBuffer;

	/**
	*	�|�[�Y���ւ̃|�C���^
	*/
	std::map<std::string, std::vector<VMDPose>>* mPoses;

	/**
	*	�{�[���̏��
	*/
	const std::vector<PMDBoneData>& mBoneDatas;

	/**
	*	�{�[���̐e�q�֌W���
	*/
	BoneTree& mBoneNode;

	/**
	*	���݂̃t���[���i���o�[
	*/
	int mFrame;

	/**
	*	���݂̃A�j���[�V�����̏I���t���[��
	*/
	int mMaxFrame;

	/**
	*	�A�j���[�V���������[�v���邩�ǂ����̃t���O
	*	ture:���[�v����	false:���[�v���Ȃ�
	*/
	bool mIsLoop;

	/**
	*	�X�V�X�e�[�g�ێ��ϐ�
	*/
	void (VMDPlayer::*mUpdate)();

	/**
	*	�I���`�F�b�N�X�e�[�g�ێ��ϐ�
	*/
	void (VMDPlayer::*mEndCheck)();

	std::vector<std::vector<VMDPose>::iterator> mCurrentPoses;

	/**
	*	@brief	�Ώۂ̃{�[������]������
	*	@param[in]	boneName	��]������{�[���̖��O
	*	@param[in]	boneRotaMatrix	�{�[����]�s��
	*/
	void VMDBoneRotation(const std::string& boneName, const DirectX::XMMATRIX& boneRotaMatrix);

	/**
	*	@brief	�ċA�I�ɑΏۂ̃{�[���̎q�{�[������]������
	*	@param[i]	parentBoneMatrix	�{�[����]�s��
	*	@param[i]	�q������]���������{�[���̃C���f�b�N�X
	*/
	void VMDBoneChildRotation(const DirectX::XMFLOAT4X4& parentBoneMatrix, int parentIndex);

	/**
	*	�I���`�F�b�N������X�e�[�g
	*/
	void EndCheck();

	/**
	*	�I���`�F�b�N�����Ȃ��X�e�[�g
	*/
	void NonCheck();

	/**
	*	�X�V��Ԃ�Update�X�e�[�g
	*/
	void PlayingUpdate();

	/**
	*	��~��Ԃ�Update�X�e�[�g
	*/
	void StopUpdate();

	/**
	*	���݂̃|�[�Y�������ݒ�ɂ���
	*/
	void PoseSet();

	/**
	*	���[�g�̈ړ���S�̂ɓK������
	*/
	void ApplyRootLocation();
};

