#pragma once
/**
*	@file VMDMotion.h
*	@brief VMDMotion�̒�`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/07
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
*	@brief VMD�̃w�b�_����
*/
struct VMDHeader
{
	/**
	*	�w�b�_����
	*/
	unsigned char header[30];

	/**
	*	�Ώۂ̃��f���̖��O
	*/
	unsigned char modelName[20];

	/**
	*	�L�[�t���[���̏�񂪉����邩
	*/
	unsigned int count;
};

/**
*	@ingroup PMD_Animation
*	@struct VMDMotionData
*	@brief ���[�V�����f�[�^�̏ڍ�
*/
struct VMDMotionData
{
	/**
	*	�{�[���̖��O
	*/
	char boneName[15];

	/**
	*	�t���[���̔ԍ�
	*/
	unsigned long frameNo;

	/**
	*	���W���
	*/
	DirectX::XMFLOAT3 location;

	/**
	*	��]���
	*/
	DirectX::XMFLOAT4 quoternion;//��]

	/**
	*	��ԃp�����[�^
	*/
	unsigned char interpolation[64];
};

/**
*	@ingroup PMD_Animation
*	@struct	VMDPose
*	@brief �|�[�Y�̃t���[���Ɖ�]���
*/
struct VMDPose
{
	/**
	*	�t���[���ԍ�
	*/
	int frameNo;

	/**
	*	��]���
	*/
	DirectX::XMFLOAT4 quoternion;
};

/**
*	@ingroup PMD_Animation
*	@class VMDMotion
*	@brief VMD�̃��[�V�����ڍׂ�ێ�����
*/
class VMDMotion :public Animation
{
	friend class VMDLoader;
	friend class VMDPlayer;
public:
	VMDMotion();
	~VMDMotion();
private:
	/**
	*	�w�b�_����ێ�
	*/
	VMDHeader mHeader;

	/**
	*	���[�V�����f�[�^��ێ�
	*/
	std::vector<VMDMotionData> mMotions;

	/**
	*	�{�[���̖��O���ƂɃ|�[�Y��ێ�
	*/
	std::map<std::string, std::vector<VMDPose>> mPoses;
};

