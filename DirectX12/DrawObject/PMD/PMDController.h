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
class VMDAnimation;
class VMDPlayer;
class DirectionalLight;
class Dx12DescriptorHeapObject;
class PipelineStateObject;
class RootSignatureObject;
class Dx12BufferObject;

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
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::shared_ptr<Dx12CommandList>& cmdList);
	~PMDController();

	/**
	*	�`�施�߂�CommandList�ɐς�
	*/
	void Draw();

	/**
	*	shadowmap�쐬�p
	*/
	void DrawShadowmap();

	/**
	*	shadow���g���`��
	*/
	void DrawShadow();

	/**
	*	@brief	VMD���[�V������ݒ肷��
	*	@param[in]	animation	���f���ɓK�����郂�[�V����
	*/
	void SetAnimation(std::shared_ptr<VMDAnimation> animation);

	/**
	*	@brief	�ݒ肵�����[�V�������Đ�����
	*	@param[in]	loopFlag	���[�v���邩�ǂ����̐ݒ� true:���[�v���� false:���[�v���Ȃ�
	*/
	void PlayAnimation(bool isLoop = false);

	/**
	*	�ݒ肵�����[�V�����̍Đ����~�߂�
	*/
	void StopAnimation();

	/**
	*	@brief	���f���ɓK�����郉�C�g��ݒ肷��
	*	@param[in]	dlight	�ݒ肷��DirectionalLight
	*/
	void SetLight(std::shared_ptr<DirectionalLight> dlight);

	/**
	*	@brief	�`�悷��ۂɎg�p����PipelineState��ݒ肷��
	*	@param[in]	pipelineState	�g�p����PipelineState
	*/
	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelineState);

	/**
	*	@brief	�`�悷��ۂɎg�p����RootSignature��ݒ肷��
	*	@param[in]	rootsignature	�ݒ肷��RootSignature
	*/
	void SetGraphicsRootSignature(std::shared_ptr<RootSignatureObject>& rootsiganture);

	/**
	*	@brief	�e�N�X�`�����g���ĕ`�悷��ۂɎg�p����PipelineState��ݒ肷��
	*	@param[in]	pipelineState	�g�p����PipelineState
	*/
	void SetToonPipelineState(std::shared_ptr<PipelineStateObject>& pipelineState);

	/**
	*	@brief	�e�N�X�`�����g���ĕ`�悷��ۂɎg�p����RootSignature��ݒ肷��
	*	@param[in]	rootsignature	�ݒ肷��RootSignature
	*/
	void SetToonRootSignature(std::shared_ptr<RootSignatureObject>& rootsiganture);

	/**
	*	shadowmap���쐬���邽�߂�rootsignature
	*/
	void SetShadowmapRootsignature(std::shared_ptr<RootSignatureObject>& rootsignature);

	/**
	*	shadowmap���쐬���邽�߂�pipelinestate
	*/
	void SetShadowmapPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	/**
	*	shadowmap���g����Toon�����_�����O���邽�߂�rootsignature
	*/
	void SetShadowRenderRootsignature(std::shared_ptr<RootSignatureObject>& rootsignature);
	
	/**
	*	shadowmap���g����Toon�����_�����O���邽�߂�pipelinestate
	*/
	void SetShadowRenderPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	/**
	*	shadowmap���g���Ēʏ탌���_�����O���邽�߂�pipelinestate
	*/
	void SetShadowBasicRenderPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	/**
	*	shadowmap���g���Ēʏ탌���_�����O���邽�߂�rootsignature
	*/
	void SetShadowBasicRenderRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);

	/**
	*	shadowmapw�쐬�p�̃R�}���h���X�g
	*/
	void SetShadowmapCommandList(std::shared_ptr<Dx12CommandList> cmdList);

	/**
	*	descriptorHeap���č\�z���� ������
	*/
	void UpdateDescriptorHeap();

	/**
	*	shadowrender�p��DescHeap���쐬����
	*/
	void CreateShadowRenderDescHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, const std::string & name);

	/**
	*	@brief	shadowmap�e�N�X�`����ݒ肷��
	*/
	void SetShadowmap(std::shared_ptr<Dx12BufferObject> shadowmap);
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
	*	toon�̃e�N�X�`�����g�p����pipeline
	*/
	std::shared_ptr<PipelineStateObject> mToonPipeline;

	/**
	*	toon�e�N�X�`�����g�p����rootsignature
	*/
	std::shared_ptr<RootSignatureObject> mToonRootsignature;

	/**
	*	shadowmap�쐬�ppipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mShadowmapPipeline;

	/**
	*	shadowmap�쐬�prootsignature
	*/
	std::shared_ptr<RootSignatureObject> mShadowmapRootsignature;

	/**
	*	toon��shadow������pipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mShadowToonRenderPipeline;

	/**
	*	toon��shadow������rootsignature
	*/
	std::shared_ptr<RootSignatureObject> mShadowToonRenderRootsignature;

	/**
	*	toon�Ȃ���shadow������pipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mShadowRenderPipeline;

	/**
	*	toon�Ȃ���shadow������rootsignature
	*/
	std::shared_ptr<RootSignatureObject> mShadowRenderRootsignature;

	/**
	*	shadowmap�쐬�pcommandlist
	*/
	std::shared_ptr<Dx12CommandList> mShadowmapCmdList;

	/**
	*	���̃R���g���[���[�Ŏg�p����DescriptorHeap
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;

	/**
	*	shadow�����_�����O�pdescriptorheap
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> mShadowRenderDescHeap;

	/**
	*	shadowmap�̃e�N�X�`��
	*/
	std::shared_ptr<Dx12BufferObject> mShadowmapTexture;

	/**
	*	DescriptorHeap�ł̃R���X�^���g�o�b�t�@�܂ł̃I�t�Z�b�g
	*/
	unsigned int mConstantBufferOffset;

	/**
	*	�e�N�X�`���̐�
	*/
	unsigned int mTextureNum;

	/**
	*	bundle�̍X�V��Ԃ����X�e�[�g�ϐ�
	*/
	void (PMDController::*mBundleUpdate)();

	/**
	*	@brief	toon���g�p���镔���Ǝg�p���Ȃ������𕪂��Ȃ���CommandList�ɖ��߂�o�^����
	*	@param[in]	cmdList		���߂�o�^����R�}���h���X�g
	*/
	void DrawWhileSetTable(std::shared_ptr<Dx12CommandList>& cmdList
	, std::pair<std::shared_ptr<PipelineStateObject>, std::shared_ptr<RootSignatureObject>> toonPair
	, std::pair<std::shared_ptr<PipelineStateObject>, std::shared_ptr<RootSignatureObject>> basicPair);

	/**
	*	@brief	�}�e���A������CommandList�ɐݒ肷��
	*	@param[in]	cmdList		�}�e���A����ݒ肷��CommandList
	*	@param[in]	resourceIndex	���Ԗڂ̃}�e���A�����̃C���f�b�N�X
	*	@param[in]	offsetCount		DescriptorHeap�̃I�t�Z�b�g�ԍ�
	*/
	void SetMaterial(std::shared_ptr<Dx12CommandList>& cmdList, unsigned int resourceIndex, unsigned int offsetCount);

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
	void SetConstantBuffers(std::shared_ptr<Dx12CommandList>& cmdList);

	/**
	*	BundelCommadnList���X�V����
	*/
	void UpdateBundle();

	/**
	*	BundleCommandList���X�V���Ȃ�
	*/
	void NonUpdateBundle();


};

