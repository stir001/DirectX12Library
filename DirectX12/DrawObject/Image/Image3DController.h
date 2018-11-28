#pragma once
/**
*	@file Image3DController.h
*	@brief 3D�̎l�p�`�|���S����2DImage�𒣂�N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/15
*/

#include "DrawObject/DrawController3D.h"
#include "ImageControllerStructure.h"
#include <DirectXMath.h>

class VertexBufferObject;
class ConstantBufferObject;
class Rect;
class ImageObject;
class Dx12DescriptorHeapObject;

/**
*	@ingroup DrawObjectController
*	@class Image3DController
*	@brief 3D��Ԃɑ��݂���Image�𑀍삷��N���X
		�傫����50�s�N�Z����1unit�Ƃ��Ă���
*/
class Image3DController :
	public DrawController3D
{
public:
	/**
	*	@param[in]	img		3D��Ԃɒu������2D�̉摜���
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	cmdList	�`��R�}���h��ςރR�}���h���X�g
	*	@param[in]	piplinestate	�g�p����pipelinestate
	*	@param[in]	rootsignature	�g�p����rootsiganture
	*/
	Image3DController(std::shared_ptr<ImageObject> img,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
		std::shared_ptr<PipelineStateObject>& pipelinestate,
		std::shared_ptr<RootSignatureObject>& rootsignature);

	~Image3DController();

	/**
	*	@brief	�摜�𕽍s�ړ�������
	*	@param[in]	x	���s�ړ���x����
	*	@param[in]	y	���s�ړ���y����
	*	@param[in]	z	���s�ړ���z����
	*/
	void AddCenterPos(const float x, const float y, const float z);

	/**
	*	@brief	�摜�𕽍s�ړ�������
	*	@param[in]	offset	���s�ړ�����
	*/
	void AddPosition(const DirectX::XMFLOAT3& offset);

	/**
	*	@brief	�摜�����ׂĂ̐�����ǉ��œ��{�Ɋg��k��������
	*	@param[in]	scale	�g�嗦�ω���
	*/
	void AddScale(const float scale);

	/**
	*	@brief	�摜���e�������Ƃɒǉ��Ŋg��k��������
	*	@param[in]	scaleX	x�����̊g�嗦�ω���
	*	@param[in]	scaleY	y�����̊g�嗦�ω���
	*	@param[in]	sclaeZ	z�����̊g�嗦�ω���
	*/
	void AddScale(const float scaleX, const float scaleY, const float scaleZ);

	/**
	*	@brief	�摜��x�����S�ŉ�]������
	*	@param[in]	deg		��]��(degree,�x���@)
	*/
	void AddRotaX(float deg);

	/**
	*	@brief	�摜��y�����S�ŉ�]������
	*	@param[in]	deg		��]��(degree,�x���@)
	*/
	void AddRotaY(float deg);

	/**
	*	@brief	�摜��z�����S�ŉ�]������
	*	@param[in]	deg		��]��(degree,�x���@)
	*/
	void AddRotaZ(float deg);

	/**
	*	@brief	�摜�̔C�ӂ̋�`������\������悤�ɐݒ肷��
	*	@param[in]	inc		�摜�̒��S��(0,0)�Ax���̐��̕������E�Ay���̐��̕��������̏�Ԃł̋�`�̒��S���W
	*	@param[in]	inw		��`�̉���
	*	@param[in]	inh		��`�̏c��
	*/
	void SetRect(const DirectX::XMFLOAT3& inc, const float inw, const float inh);

	/**
	*	@brief	�摜�̔C�ӂ̋�`������\������悤�ɐݒ肷��
	*	@param[in]	rc		�摜�̒��S��(0,0)�Ax���̐��̕������E�Ay���̐��̕��������̏�Ԃł̋�`�̏��
	*/
	void SetRect(const Rect& rc);

	/**
	*	@brief	�C�ӂ̍��W�։摜�̒��S�𕽍s�ړ�������
	*	@param[in]	x		���s�ړ�������x���W
	*	@param[in]	y		���s�ړ�������y���W
	*	@param[in]	z		���s�ړ�������z���W
	*/
	void SetPosition(const float x, const float y, const float z);

	/**
	*	@brief	�C�ӂ̍��W�։摜�̒��S�𕽍s�ړ�������
	*	@param[in]	setPos	���s�ړ���������W
	*/
	void SetPosition(const DirectX::XMFLOAT3& setPos);

	/**
	*	@brief	�摜�̊g�嗦�𓙔{�̔{���Őݒ肷��
	*	@param[in]	scale	�ݒ肷��g�嗦
	*/
	void SetScale(const float scale);

	/**
	*	@brief	�摜�̊g�嗦���e�������Ƃɐݒ肷��
	*	@param[in]	scaleX	�ݒ肷��X�����̊g�嗦
	*	@param[in]	scaleY	�ݒ肷��Y�����̊g�嗦
	*	@param[in]	scaleZ	�ݒ肷��Z�����̊g�嗦
	*/
	void SetScale(const float scaleX, const float scaleY, const float scaleZ);

	/**
	*	@brief	U�����ŉ摜�𔽓]������
	*/
	void TurnU();

	/**
	*	@brief	V�����ŉ摜�𔽓]������
	*/
	void TurnV();

	/**
	*	@brief	�摜��\������
	*/
	void Draw();

	/**
	*	@brief	�摜�̃T�C�Y���擾����
	*	@return	�摜�̃T�C�Y
	*/
	DirectX::XMFLOAT2 GetImageSize();

	/**
	*	@brief	���g�̃R�s�[���擾����
	*	@param[in]	���g�̃R�s�[	���L���͕�
	*/
	std::shared_ptr<Image3DController> GetNewCopy();

	/**
	*	@brief	�g�p����rootsignature��ݒ肷��
	*	@param[in]	rootsignature	�g�p����rootsiganture
	*/
	void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);

	/**
	*	@brief	�g�p����pipelinestate��ݒ肷��
	*	@param[in]	pipelinestate	�g�p����pipelinestate
	*/
	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	/**
	*	@brief	�g�p����R�}���h���X�g��ݒ肷��
	*	@param[in]	cmdList		�ݒ肷��R�}���h���X�g
	*/
	void SetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);

	/**
	*	@brief	�摜�̖��O���擾����
	*	@return	�摜�̖��O
	*/
	std::string GetImageName() const;

	/**
	*	@brief	�摜�̃t�@�C���p�X���擾����
	*	@return	�摜�̃t�@�C���p�X
	*/
	std::string GetFilePath() const;

	/**
	*	DescriptorHeap���č\�z����
	*/
	void UpdateDescriptorHeap();
private:
	/**
	*	@enum	eROOT_PARAMATER_INDEX
	*	@brief	���̃N���X�Ŏg�p����rootparamaterIndex
	*/
	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE,
		eROOT_PARAMATER_INDEX_CAMERA,
		eROOT_PARAMATER_INDEX_MATRIX,
	};

	/**
	*	x�g�嗦
	*/
	float mScaleX;

	/**
	*	y�g�嗦
	*/
	float mScaleY;

	/**
	*	z�g�嗦
	*/
	float mScaleZ;

	/**
	*	��]�s��
	*/
	DirectX::XMFLOAT4X4 mRotaMatrix;

	/**
	*	�A�t�B���s��
	*/
	DirectX::XMFLOAT4X4 mImageMatrix;

	/**
	*	���]���Ă��邩�ǂ����̕���(1:������	-1:�t����)
	*/
	DirectX::XMFLOAT2 mTurnSign;

	/**
	*	�`��̍ۂ̊�_(��ʍ��W)��]�┽�]�̒��S
	*/
	DirectX::XMFLOAT3 mCenter;

	/**
	*	�@�����
	*/
	DirectX::XMFLOAT4 mNormal;

	/**
	*	���_���
	*/
	Image3DVertex mVertex[4];

	/**
	*	���_�p�̃o�b�t�@
	*/
	std::shared_ptr<VertexBufferObject> mVertexBuffer;

	/**
	*	�A�t�B���s��p�̃o�b�t�@
	*/
	std::shared_ptr<ConstantBufferObject> mImageMatrixBuffer;
	
	/**
	*	�摜���̐؂蔲�����[�J�����W��`
	*/
	std::shared_ptr<Rect> mRect;

	/**
	*	2D�摜��������
	*/
	std::shared_ptr<ImageObject> mImgObj;

	/**
	*	DescriptorHeap
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;

	/**
	*	bundle�p�̃A�b�v�f�[�g�X�e�[�g�ϐ�
	*/
	void (Image3DController::*mBundleUpdate)();

	/**
	*	UV���X�V����
	*/
	void UpdateUV();

	/**
	*	�A�t�B���s����X�V����
	*/
	void UpdateMatrix();

	/**
	*	�A�t�B���s����o�b�t�@�ɏ�������
	*/
	void UpdateBuffer();

	/**
	*	�o���h�����X�V����
	*/
	void UpdateBundle();

	/**
	*	�o���h���̍X�V�����Ȃ�
	*/
	void NonUpdateBundle();
};

