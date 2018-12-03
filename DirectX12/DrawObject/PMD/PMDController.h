#pragma once
/**
*	@file PMDController.h
*	@brief PMDModel�𑀍삷��N���X�̒�`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/15
*/
#include "DirectX12/DrawObject/DrawController3D.h"
#include "PMDDataStructure.h"

class PMDModel;
class ConstantBufferObject;
class VMDMotion;
class VMDPlayer;
class DirectionalLight;
class Dx12DescriptorHeapObject;
class PipelineStateObject;
class RootSignatureObject;

/**
*	@ingroup Model
*	@class PMDController
*	@brief PMDModel�𑀍삷��N���X
*/
class PMDController
	: public DrawController3D
{
	friend class PMDLoader;
public:
	/**
	*	@param[in]	model	PMD�̃f�[�^���
	*	@param[in]	dlight	���̃��f���p�̌����f�[�^
	*	@param[in]	name	���̃��f���̖��O
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	cmdList	���̃��f���̕`�施�߂�ςރR�}���h���X�g
	*/
	PMDController(std::shared_ptr<PMDModel>& model, std::shared_ptr<DirectionalLight>& dlight, const std::string& name,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev,Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	~PMDController();

	/**
	*	�`�施�߂�CommandList�ɐς�
	*/
	void Draw();

	/**
	*	@brief	VMD���[�V������ݒ肷��
	*	@param[in]	motion	���f���ɓK�����郂�[�V����
	*/
	void SetMotion(std::shared_ptr<VMDMotion> motion);

	/**
	*	@brief	�ݒ肵�����[�V�������Đ�����
	*	@param[in]	loopFlag	���[�v���邩�ǂ����̐ݒ� true:���[�v���� false:���[�v���Ȃ�
	*/
	void PlayMotion(bool isLoop = false);

	/**
	*	�ݒ肵�����[�V�����̍Đ����~�߂�
	*/
	void StopMotion();

	/**
	*	@brief	���f���ɓK�����郉�C�g��ݒ肷��
	*	@param[in]	dlight	�ݒ肷��DirectionalLight
	*/
	void SetLight(std::shared_ptr<DirectionalLight> dlight);

	/**
	*	@brief	�}�e���A�����g���ĕ`�悷��ۂɎg�p����PipelineState��ݒ肷��
	*	@param[in]	pipelineState	�g�p����PipelineState
	*/
	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelineState);

	/**
	*	@brief	�}�e���A�����g���ĕ`�悷��ۂɎg�p����RootSignature��ݒ肷��
	*	@param[in]	rootsignature	�ݒ肷��RootSignature
	*/
	void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsiganture);

	/**
	*	@brief	�e�N�X�`�����g���ĕ`�悷��ۂɎg�p����PipelineState��ݒ肷��
	*	@param[in]	pipelineState	�g�p����PipelineState
	*/
	void SetSubPipelineState(std::shared_ptr<PipelineStateObject>& pipelineState);

	/**
	*	@brief	�e�N�X�`�����g���ĕ`�悷��ۂɎg�p����RootSignature��ݒ肷��
	*	@param[in]	rootsignature	�ݒ肷��RootSignature
	*/
	void SetSubRootSignature(std::shared_ptr<RootSignatureObject>& rootsiganture);

	void UpdateDescriptorHeap();
private:
	/**
	*	PMD���f�����
	*/
	std::shared_ptr<PMDModel> mModel;

	/**
	*	�{�[���̉�]�����������ރo�b�t�@
	*/
	std::shared_ptr<ConstantBufferObject> mBoneMatrixBuffer;

	/**
	*	�{�[���̉�]���
	*/
	std::vector<DirectX::XMFLOAT4X4> mBoneMatrix;

	/**
	*	���f����DirectionalLight
	*/
	std::shared_ptr<DirectionalLight> mDirLight;

	/**
	*	VMD���Đ�����N���X
	*/
	std::shared_ptr<VMDPlayer> mVmdPlayer;

	/**
	*	�e�N�X�`�����g�p����Ƃ��Ɏg��PipelineState
	*/
	std::shared_ptr<PipelineStateObject> mSubPipeline;

	/**
	*	�e�N�X�`�����g�p����Ƃ��Ɏg��RootSignature
	*/
	std::shared_ptr<RootSignatureObject> mSubRootsignature;

	/**
	*	���̃R���g���[���[�Ŏg�p����DescriptorHeap
	*/
	std::unique_ptr<Dx12DescriptorHeapObject> mDescHeap;

	/**
	*	bundle�̍X�V��Ԃ����X�e�[�g�ϐ�
	*/
	void (PMDController::*mBundleUpdate)();

	/**
	*	@brief	�}�e���A����e�N�X�`�����g�p���镔���𕪂��Ȃ���CommandList�ɖ��߂�o�^����
	*	@param[in]	cmdList		���߂�o�^����R�}���h���X�g
	*/
	void DrawWhileSetTable(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);

	/**
	*	@brief	�}�e���A���ɐݒ肳��Ă���e�N�X�`����CommandList�ɐݒ肷��
	*	@param[in]	cmdList		�e�N�X�`����ݒ肷��CommandList
	*	@param[in]	matarial	�e�N�X�`������ێ����Ă���}�e���A��
	*/
	void SetTexture(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, PMDMaterial& material);

	/**
	*	@brief	�}�e���A������CommandList�ɐݒ肷��
	*	@param[in]	cmdList		�}�e���A����ݒ肷��CommandList
	*	@param[in]	resourceIndex	���Ԗڂ̃}�e���A�����̃C���f�b�N�X
	*	@param[in]	offsetCount		DescriptorHeap�̃I�t�Z�b�g�ԍ�
	*/
	void SetMaterial(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, unsigned int resourceIndex, unsigned int offsetCount);

	/**
	*	@brief	���݂̏����g����DescriptorHeap���쐬����
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	name	DescriptorHeap�̖��O
	*/
	void CreateDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, const std::string& name);

	/**
	*	@brief	���̃��f����ConstantBuffer��CmdList�ɐݒ肷��
	*	@param[in]	cmdList		ConstantBuffer��ݒ肷��CommandList
	*/
	void SetConstantBuffers(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);

	/**
	*	BundelCommadnList���X�V����
	*/
	void UpdateBundle();

	/**
	*	BundleCommandList���X�V���Ȃ�
	*/
	void NonUpdateBundle();
};

