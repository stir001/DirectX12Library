#pragma once
/**
*	@addtogroup Dx12Camera
*	@file Dx12Camera.h
*	@brief DirectX12�Ŏg�p����J�����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/15
*/

#include <DirectXMath.h>
#include <wrl.h	>
#include <memory>
#include <d3d12.h>

class ConstantBufferObject;
class DxInput;
struct ID3D12GraphicsCommandList;


/**
*	@ingroup Dx12Camera
*	@struct Dx12CameraBuffer
*	@brief GPU�ɓ����邽�߂̗v�f���܂Ƃ߂��\����
*/
struct Dx12CameraBuffer
{
	DirectX::XMFLOAT4 eye;
	DirectX::XMFLOAT4 target;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

/**
*	@ingroup Dx12Camera
*	@class Dx12Camera
*	@brief DirectX12�Ŏg�����߂̋@�\�����J�����N���X
*/
class Dx12Camera
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
	Dx12Camera(int wWidth, int wHeight, DirectX::XMFLOAT3& eye, DirectX::XMFLOAT3& target, DirectX::XMFLOAT3& upper);
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
	*	@brief ScisorRect��ݒ肷�� ViewPort���̐؂蔲����`
	*	@param right	�؂蔲����`�̉E��X���W
	*	@param bottom	�؂蔲����`�̉�Y���W
	*	@param left		�؂蔲����`�̍�X���W
	*	@param top		�؂蔲����`�̏�Y���W
	*/
	void SetScisorRect(int right, int bottom,
		int left = 0.0f, int top = 0.0f );

	D3D12_VIEWPORT GetViewPort() const;

	D3D12_RECT GetScisorRect() const;

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
	*	@brief	�J�����̎��R���X�^���g�o�b�t�@�𓾂�
	*	@return	�J�����̂��R���X�^���g�o�b�t�@
	*/
	std::shared_ptr<ConstantBufferObject>& GetCameraBuffer();
private:
	/**
	*	�J������GPU�ɓ�����v�f
	*/
	Dx12CameraBuffer mElement;

	/**
	*	Dx12CameraBuffer�̗v�f���������ރR���X�^���g�o�b�t�@
	*/
	std::shared_ptr<ConstantBufferObject> mCameraBuffer;

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
	*	
	*/
	D3D12_VIEWPORT mViewPort;

	/**
	*	
	*/
	D3D12_RECT mScisorRect;

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
	*	ConstantBuffer��mElement�̓��e����������
	*/
	void UpdateBuffer();
};
