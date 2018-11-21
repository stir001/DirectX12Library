#pragma once
/**
*	@file ImageController.h
*	@brief 2D�摜�̑�����s���N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/21
*/
#include "DrawObject/DrawObjectController.h"
#include "ImageControllerStructure.h"

#include <memory>
#include <DirectXMath.h>

class ImageObject;
class Rect;
class VertexBufferObject;
class Dx12DescriptorHeapObject;

/**
*	@ingroup DrawObjectController
*	@class ImageController
*	@brief 2D�摜�𑀍삷��N���X
*/
class ImageController :public DrawObjectController
{
public:
	/**
	*	@param[in]	img		2D�摜���
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	cmdList	�R�}���h��ςރR�}���h���X�g
	*	@param[in]	pipelinestate	�g�p����pipelinestate
	*	@param[in]	rootsignature	�g�p����rootsignature
	*/
	ImageController(std::shared_ptr<ImageObject> img,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> uicmdList,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> backcmdList,
		std::shared_ptr<PipelineStateObject>& pipelinestate,
		std::shared_ptr<RootSignatureObject>& rootsignature);

	~ImageController();

	/**
	*	@brief	�摜�𕽍s�ړ�������
	*	@param[in]	x	x�̕��s�ړ�����
	*	@param[in]	y	y�̕��s�ړ�����
	*	@param[in]	z	z�̕��s�ړ�����
	*/
	void AddPos(const float x, const float y, const float z);

	/**
	*	@brief	�摜�𕽍s�ړ�������
	*	@param[in]	offset	���s�ړ�����
	*/
	void AddPos(const DirectX::XMFLOAT3& offset);

	/**
	*	@brief	�摜�̊g�嗦��ω�������
	*	@param[in]	scale	�摜�̊g�嗦�̕ω���
	*/
	void AddScale(const float scale);

	/**
	*	@brief	�摜����]������
	*	@param[in]	deg		��]��(degree,�x���@)
	*/
	void AddRota(const float deg);

	/**
	*	@brief	�摜�̊�_�𕽍s�ړ�������
	*	@param[in]	offset	���s�ړ�����
	*/
	void AddPivot(const DirectX::XMFLOAT3& offset);

	/**
	*	@brief	�摜�̒��̔C�ӂ̋�`������`�悷�镔����ݒ肷��
	*	@param[in]	inc		�`�悵������`�̒��S�_
	*	@param[in]	inw		��`�̏c��
	*	@param[in]	inh		��`�̏c��
	*/
	void SetRect(const DirectX::XMFLOAT3& inc, const float inw, const float inh);

	/**
	*	@brief	�摜�̒��̔C�ӂ̋�`������`�悷�镔����ݒ肷��
	*	@param[in]	rc		�`�悵������`�̏��
	*/
	void SetRect(const Rect& rc);

	/**
	*	@brief	�摜�̊�_��C�ӂ̏ꏊ�֕��s�ړ�������
	*	@param[in]	x		x�̕��s�ړ�����
	*	@param[in]	y		y�̕��s�ړ�����
	*	@param[in]	z		z�̕��s�ړ�����
	*/
	void SetPos(const float x, const float y, const float z);

	/**
	*	@brief	�摜�̊�_��C�ӂ̏ꏊ�֕��s�ړ�������
	*	@param[in]	offset	���s�ړ�����
	*/
	void SetPos(const DirectX::XMFLOAT3& offset);

	/**
	*	@brief	�摜�̊g�嗦�𓙔{�Őݒ肷��
	*	@param[in]	scale	�ݒ肷��g�嗦
	*/
	void SetScale(const float scale);

	/**
	*	@brief	�摜�̊g�嗦���c���ʂɐݒ肷��
	*	@param[in]	scale	�ݒ肷��g�嗦(scale.x : u�����̊g�嗦�Ascale.y : v�����̊g�嗦)
	*/
	void SetScale(const DirectX::XMFLOAT2& scale);

	/**
	*	@brief	�摜�̉�]��ݒ肷��
	*	@param[in]	deg		�ݒ肵������]�p�x(degree,�x���@)
	*/
	void SetRota(const float deg);

	/**
	*	@brief	�摜�̒��S�_�̊�_����̃I�t�Z�b�g��ݒ肷��
	*	@param[in]	x		x�����̃I�t�Z�b�g��
	*	@param[in]	y		y�����̃I�t�Z�b�g��
	*	@param[in]	z		z�����̃I�t�Z�b�g��
	*/
	void SetCenterOffset(const float x, const float y, const float z);

	/**
	*	@brief	�摜�̒��S�_�̊�_����̃I�t�Z�b�g��ݒ肷��
	*	@param[in]	offset	�I�t�Z�b�g��
	*/
	void SetCenterOffset(const DirectX::XMFLOAT3& offset);

	/**
	*	U�����ɔ��]������
	*/
	void TurnU();

	/**
	*	V�����ɔ��]������
	*/
	void TurnV();

	/**
	*	�摜��UI���C���[�ɕ`�悷��
	*/
	void Draw();

	/**
	*	�摜��BackGround���C���[�ɕ`�悷��
	*/
	void BackDraw();

	/**
	*	U�����ɔ��]���Ă��邩�ǂ����𓾂�
	*		ture : ���]���Ă��� 
	*		false : ���]���Ă��Ȃ�
	*/
	bool IsTurnU() const;

	/**
	*	V�����ɔ��]���Ă��邩�ǂ����𓾂�
	*		ture : ���]���Ă���
	*		false : ���]���Ă��Ȃ�
	*/
	bool IsTurnV() const;

	/**
	*	@brief	�摜�T�C�Y���擾����
	*	@return	�摜�T�C�Y
	*/
	DirectX::XMFLOAT2 GetImageSize();

	/**
	*	@brief	���g�̃R�s�[���擾����
	*	@return	���g�̃R�s�[(���L���͕�)
	*/
	std::shared_ptr<ImageController> Duplicate();

	/**
	*	@brief	�g�p����Rootsiganture��ݒ肷��
	*	@param[in]	rootsignature	�g�p����rootsignature��ݒ肷��
	*/
	void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);

	/**
	*	@brief	�g�p����Rootsiganture��ݒ肷��
	*	@param[in]	pipelinestate	�g�p����pipelinestate��ݒ肷��
	*/
	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	/**
	*	@brief	�g�p����R�}���h���X�g��ݒ肷��
	*	@param[in]	cmdList		�g�p����R�}���h���X�g
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
private:
	/**
	*	�����̊g�嗦
	*/
	float mScaleX;

	/**
	*	�c���̊g�嗦
	*/
	float mScaleY;

	/**
	*	���݂̉�]�p�x
	*/
	float mRota;

	/**
	*	�摜�̒��S����̊e���_�ւ̃x�N�g���̒���
	*/
	float mLength[4];

	/**
	*	���]���Ă��邩�ǂ����̒l
	*/
	DirectX::XMFLOAT2 mTurnSign;

	/**
	*	�`�悷��摜�̒��S�̕`���_����̃I�t�Z�b�g
	*/
	DirectX::XMFLOAT3 mCenterOffset;

	/**
	*	�摜�̒��S����̊e���_�ւ̒P�ʃx�N�g��
	*/
	DirectX::XMFLOAT3 mNormvec[4];

	/**
	*	�`��̍ۂ̊�_(��ʍ��W)��]�┽�]�̒��S
	*/
	DirectX::XMFLOAT3 mPivot;

	/**
	*	���_���
	*/
	ImageVertex mVertex[4];

	/**
	*	���_�����������ރo�b�t�@
	*/
	std::shared_ptr<VertexBufferObject> mVertexBuffer;

	/**
	*	�摜���̐؂蔲�����[�J�����W��`
	*/
	std::shared_ptr<Rect> mRect;

	/**
	*	����rootsiganture�Ŏg�����[�g�p�����[�^�[
	*/
	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE,
	};

	/**
	*	2D�摜�̏��
	*/
	std::shared_ptr<ImageObject> mImgObj;

	/**
	*	���̉摜�p��DescriptorHeap
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;

	/**
	*	�w�i�`��p�̃R�}���h���X�g
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mBackCmdList;

	/**
	*	�o���h���̍X�V�������X�e�[�g�ϐ�
	*/
	void (ImageController::*mBundleUpdate)();

	/**
	*	UV�����X�V����
	*/
	void UpdateUV();

	/**
	*	�摜�̒��S����e���_�ւ̒P�ʃx�N�g�����X�V����
	*/
	void UpdateNormvec();

	/**
	*	���_�o�b�t�@���X�V����
	*/
	void UpdateBuffer();

	/**
	*	�o���h�����X�V����
	*/
	void UpdateBundle();

	/**
	*	�o���h�����X�V���Ȃ�
	*/
	void NonUpdateBundle();
};

