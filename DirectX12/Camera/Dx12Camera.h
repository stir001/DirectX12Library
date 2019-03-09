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
	void SetPos(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	�J�����̍��W��ݒ肷��
	*	@param[in]	x	�ݒ肷��x���W
	*	@param[in]	y	�ݒ肷��y���W
	*	@param[in]	z	�ݒ肷��z���W
	*/
	void SetPos(float x, float y, float z);

	/**
	*	@brief	�J�����̒����_��ݒ肷��
	*	@param[in]	target	�ݒ肷����W
	*/
	void SetTarget(const DirectX::XMFLOAT3& target);

	/**
	*	@brief	�J�����̒����_��ݒ肷��
	*	@param[in]	x	�ݒ肷�钍���_x���W
	*	@param[in]	y	�ݒ肷�钍���_y���W
	*	@param[in]	z	�ݒ肷�钍���_z���W
	*/
	void SetTarget(float x, float y, float z);

	/**
	*	@brief	�����̕�����ς��Ȃ��悤��
				�J�����̍��W��ݒ肷��(�����_���ꏏ�ɂ����)
	*	@param[in]	pos		�ݒ肷����W
	*/
	void ParallelSetPos(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	���[���h���X����]����
	*	@param[in]	rad		��]��(radian,�ʓx�@)
	*/
	void AddXAxisRota(float rad);

	/**
	*	@brief	���[���h���Y����]����
	*	@param[in]	rad		��]��(radian,�ʓx�@)
	*/
	void AddYAxisRota(float rad);

	/**
	*	@brief	���[���h���Z����]����
	*	@param[in]	rad		��]��(radian,�ʓx�@)
	*/
	void AddZAxisRota(float rad);

	/**
	*	@brief	y�����ֈړ�����
	*	@param[in]	val	�ړ������̑傫��
	*/
	void MoveUp(float val);

	/**
	*	@brief	�J�����̃��[�J���O�����ֈړ�����
	*	@param[in]	val	�ړ������̑傫��
	*/
	void MoveFront(float vel);

	/**
	*	@brief	XZ���ʏ�̃��[�J���������ֈړ�����
	*	@param[in]	val	�ړ������̑傫��
	*/
	void MoveSide(float vel);

	/**
	*	@brief	Y�����S�ŉ�]����
	*	@param[in]	rad	��]�����̑傫��(radian,�ʓx�@)
	*/
	void TurnRightLeft(float rad);

	/**
	*	@brief	�J�����̃��[�J��������։�]����
	*	@param[in]	rad	��]�����̑傫��(radian,�ʓx�@)
	*/
	void TurnUpDown(float rad);

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
	*	@brief	viewpor�̒l����ʔ�Ō��肷�� ����(0,0) �E��(1,1)
	*	@param[in]	left		���[x��ʍ��W��
	*	@param[in]	top			��[y��ʍ��W��
	*	@param[in]	right		�E�[x��ʍ��W��
	*	@param[in]	bottom		���[y��ʍ��W��
	*	@param[in]	minDepth	�ŏ�depth�l
	*	@param[in]	maxDepth	�ő�depth�l
	*/
	void SetViewPortNormalizeValue(float left, float top
		, float right, float bottom
		, float minDepth = 0.0f, float maxDepth = 1.0f);

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
	*	@brief ScisorRect����ʔ�Ō��肷�� 
	*		0~���� : 0 ~ 1.0, 0~�c�� : 0~1.0
	*	@param[in]	left	���[�̉�ʔ�(0~1.0)
	*	@param[in]	top		��[�̉�ʔ�(0~1.0)
	*	@param[in]	right	�E�[�̉�ʔ�(0~1.0)
	*	@param[in]	bottom	���[�̉�ʔ�(0~1.0)
	*/
	void SetScisorRectNormalizeValue(float left, float top
		,float right, float bottom);

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

	/**
	*	@brief fov��ݒ肷��
	*	@param[in]	rad		����p(radian,�ʓx�@)
	*/
	void SetFoV(float rad);

	/**
	*	@brief	�N���b�s���O��near��
	*	@param[in]	cameraNear	�j�A�̋���
	*/
	void SetNear(float cameraNear);

	/**
	*	@brief	�J������
	*/
	void SetFar(float cameraFar);

	/**
	*	��ʂ̑傫���ɍ��킹��viewport��scissorRect���X�V����(��ʔ�x�[�X)
	*/
	void UpdateViewportScisoorRect();
	/**
	*	@brief	�J�������Ή����Ă���viewport�̃T�C�Y���擾����
	*/
	DirectX::XMINT2 GetViewPortSize() const;

	/**
	*	@brief	fov���擾����
	*	@return	���݂�fov
	*/
	float GetFov() const;
private:
	/**
	*	@struct	Viewport��ScissorRect�̉�ʔ�ۑ��p�\����
	*/
	struct RectRatio
	{
		float left;		//!	���[x��ʍ��W�䗦
		float top;		//!	��[y��ʍ��W�䗦
		float right;	//!	�E�[x��ʍ��W�䗦
		float bottom;	//!	���[y��ʍ��W�䗦
		RectRatio() :left(0.0f), top(0.0f), right(1.0f), bottom(1.0f) {}
		RectRatio(float left, float top, float right, float bottom)
			: left(left), top(top), right(right), bottom(bottom) {}
	};

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
	*	viewPort�̉�ʔ䗦
	*/
	RectRatio mViewPortRatio;

	/**
	*	�`��͈͓��̐؂蔲����`
	*/
	D3D12_RECT mScissorRect;

	/**
	*	sciddorRect�̉�ʔ䗦�ۑ��p
	*/
	RectRatio mScissorRectRatio;

	/**
	*	������ێ����Ă���z���_�[
	*/
	std::weak_ptr<CameraHolder> mHolder;

	/**
	*	�����̊Ǘ�����Ă���C���f�b�N�X
	*/
	int mHoldIndex;

	/**
	*	����p
	*/
	float mFov;

	/**
	*	nearClipping
	*/
	float mNear;

	/**
	*	farClipping
	*/
	float mFar;

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

	/**
	*	projection�s����X�V����
	*/
	void UpdateProjection();

	/**
	*	viewport��sissorRect�̒l�Ɖ�ʃT�C�Y���Q�l�ɔ䗦���X�V����
	*/
	void UpdateRatios();
};
