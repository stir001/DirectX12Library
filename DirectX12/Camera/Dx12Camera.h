#pragma once
/**
*	@addtogroup Dx12Camera
*	@file Dx12Camera.h
*	@brief DirectX12�Ŏg�p����J�����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/23
*/
#include "CameraBufferStructure.h"

#include <DirectXMath.h>
#include <wrl.h	>
#include <memory>
#include <d3d12.h>

class ConstantBufferObject;
class DxInput;
class CameraHolder;
struct ID3D12GraphicsCommandList;

/**
*	@ingroup Dx12Camera
*	@class Dx12Camera
*	@brief DirectX12�Ŏg�����߂̋@�\�����J�����N���X
*/
class Dx12Camera : public std::enable_shared_from_this<Dx12Camera>
{
public:
	/**
	*	@param[in]	width	�J�����̉f���s�N�Z���P�ʂ̉���
	*	@param[in]	height	�J�����̉f���s�N�Z���P�ʂ̏c��
	*/
	Dx12Camera(int wWidth, int wHeight);

	/**
	*	@param[in]	width	�J�����̉f���s�N�Z���P�ʂ̉���
	*	@param[in]	height	�J�����̉f���s�N�Z���P�ʂ̏c��
	*	@param[in]	eye		�J�����̈ʒu
	*	@param[in]	target	�J�����̒����_
	*	@param[in]	upper	�J�����̏�x�N�g��(world���W�)
	*/
	Dx12Camera(int wWidth, int wHeight, const DirectX::XMFLOAT3& eye,
		const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& upper);

	/**
	*	@param[in]	width	�J�����̉f���s�N�Z���P�ʂ̉���
	*	@param[in]	height	�J�����̉f���s�N�Z���P�ʂ̏c��
	*	@param[in]	eye		�J�����̈ʒu
	*	@param[in]	target	�J�����̒����_
	*	@param[in]	upper	�J�����̏�x�N�g��(world���W�)
	*/
	Dx12Camera(D3D12_VIEWPORT viewport, D3D12_RECT scissorRect, const DirectX::XMFLOAT3& eye,
		const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& upper,
		std::shared_ptr<CameraHolder> holder, unsigned int holdIndex);
	~Dx12Camera();

	/**
	*	@brief	�J�����̍��W��ݒ肷��
	*	@param[in]	pos		�ݒ肷����W
	*/
	void SetPos(DirectX::XMFLOAT3& pos);

	/**
	*	@brief	�J�����̒����_��ݒ肷��
	*	@param[in]	target	�ݒ肷����W
	*/
	void SetTarget(DirectX::XMFLOAT3& target);

	/**
	*	@brief	���[���h���X����]����
	*	@param[in]	deg		��]��(degree,�x���@)
	*/
	void AddXAxisRota(float deg);

	/**
	*	@brief	���[���h���Y����]����
	*	@param[in]	deg		��]��(degree,�x���@)
	*/
	void AddYAxisRota(float deg);

	/**
	*	@brief	���[���h���Z����]����
	*	@param[in]	deg		��]��(degree,�x���@)
	*/
	void AddZAxisRota(float deg);

	/**
	*	@brief	�J�����̃��[�J��������ֈړ�����
	*	@param[in]	val	�ړ������̑傫��
	*/
	void MoveUp(float val);

	/**
	*	@brief	�J�����̃��[�J���O�����ֈړ�����
	*	@param[in]	val	�ړ������̑傫��
	*/
	void MoveFront(float vel);

	/**
	*	@brief	�J�����̃��[�J���������ֈړ�����
	*	@param[in]	val	�ړ������̑傫��
	*/
	void MoveSide(float vel);

	/**
	*	@brief	�J�����̃��[�J���������։�]����
	*	@param[in]	deg	��]�����̑傫��(degree,�x���@)
	*/
	void TurnRightLeft(float deg);

	/**
	*	@brief	�J�����̃��[�J��������։�]����
	*	@param[in]	deg	��]�����̑傫��(degree,�x���@)
	*/
	void TurnUpDown(float deg);

	/**
	*	@brief ViewPort��ݒ肷�� �`�悷���ʓ��̑傫��
	*	@param width	�`��͈͂̉���
	*	@param height	�`��͈͂̏c��
	*	@param topLX	�`��͈͂̍���X���W
	*	@param topLY	�`��͈͂̍���Y���W
	*	@param minDepth	�[�x�̍ŏ��l
	*	@param maxDepth	�[�x�̍ő�l
	*/
	void SetViewPort(float width, float height,
		float topLX = 0.0f, float topLY = 0.0f,
		float minDepth = 0.0f, float maxDepth = 1.0f);

	/**
	*	@brief ScisorRect��ݒ肷�� ViewPort���̐؂蔲����`(��ʍ��W)
	*	@param right	�؂蔲����`�̉E��X���W
	*	@param bottom	�؂蔲����`�̉�Y���W
	*	@param left		�؂蔲����`�̍�X���W
	*	@param top		�؂蔲����`�̏�Y���W
	*/
	void SetScisorRect(int right, int bottom,
		int left = 0.0f, int top = 0.0f );

	/**
	*	�J������ViewPort���擾����
	*/
	D3D12_VIEWPORT GetViewPort() const;

	/**
	*	�J������ScisorRect���擾����
	*/
	D3D12_RECT GetScissorRect() const;

	/**
	*	@brief	���炩���ߒ񋟂���Ă���ړ��ŃJ�������ړ�������
	*	@param[in]	input	���݂̓��͏��
	*/
	void DefaultMove(const DxInput& input);

	/**
	*	@brief	�J������View�s��𓾂�
	*	@return	�J������View�s��
	*/
	DirectX::XMFLOAT4X4 GetView();

	/**
	*	@brief	�J�����̃v���W�F�N�V�����s��𓾂�
	*	@return	�J�����̃v���W�F�N�V�����s��
	*/
	DirectX::XMFLOAT4X4 GetProjection();

	/**
	*	@brief	�J�����̍��W�𓾂�
	*	@return	�J�����̍��W
	*/
	DirectX::XMFLOAT4 GetCameraPosition();

	/**
	*	@brief	�J�����̃��[���h�s��𓾂�
	*	@return	�J�����̃��[���h�s��
	*/
	DirectX::XMFLOAT4X4 GetWorld();

	/**
	*	�J���������擾����
	*/
	Dx12CameraBuffer GetCameraBufferElement();

	/**
	*	�J�������ێ�����Ă���C���f�b�N�X
	*/
	int GetHoldIndex() const;
private:
	/**
	*	�J������GPU�ɓ�����v�f
	*/
	Dx12CameraBuffer mElement;

	/**
	*	�J������View�s��
	*/
	DirectX::XMFLOAT4X4 mCamera;

	/**
	*	�J������Projection�s��
	*/
	DirectX::XMFLOAT4X4 mProjection;

	/**
	*	�J�����̏�x�N�g��
	*/
	DirectX::XMFLOAT3 mUpper;

	/**
	*	�J�����̃��[���h�s��
	*/
	DirectX::XMFLOAT4X4 mWorldRotation;

	/**
	*	�J�����̃��[�J����x�N�g��
	*/
	DirectX::XMFLOAT3 mLocalUpper;

	/**
	*	�J�����̉f���s�N�Z���P�ʂ̉���
	*/
	int mWidth;

	/**
	*	�J�����̉f���s�N�Z���P�ʂ̏c��
	*/
	int mHeight;

	/**
	*	ConstantBuffer��byte�P�ʂ̃T�C�Y
	*/
	int mBuffersize;

	/**
	*	��ʂ̕`��͈͋�`
	*/
	D3D12_VIEWPORT mViewPort;

	/**
	*	�`��͈͓��̐؂蔲����`
	*/
	D3D12_RECT mScissorRect;

	/**
	*	������ێ����Ă���z���_�[
	*/
	std::weak_ptr<CameraHolder> mHolder;

	/**
	*	�����̊Ǘ�����Ă���C���f�b�N�X
	*/
	int mHoldIndex;

	/**
	*	�X�V��CameraHolder�ɒʒm����X�e�[�g�p
	*/
	void (Dx12Camera::*mHolderSetter)();

	/**
	*	@brief	�J�����ɉ�]�s���K������
	*	@param[in]	rotaMatrix	��]�s��
	*/
	void AddRotationAxis(const DirectX::XMMATRIX& rotaMatrix);

	/**
	*	@brief	���[�J����x�N�g���𓾂�
	*	@return	���[�J����x�N�g��
	*/
	DirectX::XMFLOAT3 GetLocalUpper();

	/**
	*	@brief	�����x�N�g���𓾂�
	*	@return	�����x�N�g��
	*/
	DirectX::XMFLOAT3 GetEyeToTargetVec();

	/**
	*	�V�F�[�_�[�ɓn���v�f���X�V����
	*/
	void UpdateElement();

	/**
	*	����������
	*/
	void Init();

	/**
	*	holder�ɃJ�������ύX��̒l��ݒ肷����
	*/
	void SetElementToHolder();

	/**
	*	holder�ɃJ�������ύX��̒l��ݒ肵�Ȃ����
	*/
	void NonSetElementToHolder();
};
