#pragma once
/**
*	@file PMDLoader.h
*	@brief PMD���f���̏������[�h���ێ�����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/12/12
*/
#include "DirectX12/DrawObject/DrawObjectLoader.h"
#include <map>

class PMDModel;
class PMDController;
class DirectionalLight;

/**
*	@ingroup DrawObjectLoader
*	@class PMDLoader
*	@brief PMD�����[�h���ێ�����N���X
*/
class PMDLoader :
	public DrawObjectLoader
{
public:
	PMDLoader();
	~PMDLoader();

	/**
	*	@brief	PMD�����[�h��PMD�𑀍삷��R���g���[���[��Ԃ�
	*	@param[in]	path	���[�h����PMD���f���̃t�@�C���p�X
	*	@return	PMD�𑀍삷��R���g���[���[
	*/
	std::shared_ptr<PMDController> Load(const std::string& path);
private:

	/**
	*	���[�h�������f���ɓK�����郉�C�g
	*/
	std::shared_ptr<DirectionalLight> mLight;

	/**
	*	���[�h�������f���ۑ��p
	*/
	std::map<std::string, std::shared_ptr<PMDModel>> mModels;

	/**
	*	���݃��[�h���Ă��郂�f���̈ꎞ�ۑ��p
	*/
	std::shared_ptr<PMDModel> mLoadingmodel;

	/**
	*	�e�N�X�`�����g���ĕ`�悷��Ƃ��Ɏg���V�F�[�_�[���
	*/
	ShaderDatas mSubShader;

	/**
	*	Toon�e�N�X�`�����g���ĕ`�悷��Ƃ��Ɏg��rootsiganture
	*/
	std::shared_ptr<RootSignatureObject> mToonRootsiganture;

	/**
	*	Toon�e�N�X�`�����g���ĕ`�悷��Ƃ��Ɏg��Pipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mToonPipelineState;

	/**
	*	Header���������[�h
	*/
	void LoadHeader();

	/**
	*	���_��񕔕������[�h
	*/
	void LoadVertex();

	/**
	*	�C���f�b�N�X�������[�h
	*/
	void LoadIndex();

	/**
	*	�}�e���A���������[�h
	*/
	void LoadMaterial();

	/**
	*	�{�[���������[�h
	*/
	void LoadBone();

	/**
	*	IK�������[�h
	*/
	void LoadIkList();

	/**
	*	�\��������[�h
	*/
	void LoadSkin();

	/**
	*	�\��\���������[�h
	*/
	void LoadSkinDisp();

	/**
	*	�{�[���t���[���\���������[�h
	*/
	void LoadBoneFrameDisp();

	/**
	*	�{�[���������[�h
	*/
	void LoadBoneDisp();

	/**
	*	�p��̏��@���X�L�b�v
	*/
	void SeekEngligh();

	/**
	*	�g�D�[���������[�h
	*/
	void LoadToonPath();

	/**
	*	���̏������[�h
	*/
	void LoadRigidBody();

	/**
	*	�W���C���g�������[�h
	*/
	void LoadJoint();

	/**
	*	���f���ɑΉ�����IndexBuffer���쐬
	*/
	void CreateIndexBuffer();

	/**
	*	���f���ɑΉ�����VertexBuffer���쐬
	*/
	void CreateVertexBuffer();

	/**
	*	�e�N�X�`�������[�h��ShaderResource���쐬
	*/
	void CreateTexture();

	/**
	*	toonTexture���쐬����
	*/
	void CreateToonTexture();

	/**
	*	�}�e���A���p�̃o�b�t�@���쐬
	*/
	void CreateMaterialBuffer();

	/**
	*	@brief	���[�h�������f���Ɏg�p����Pipelinestate���쐬
	*	@param[in]	dev		ID3D12Device�̎Q��
	*/
	void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	@brief	���[�h�������f���Ɏg�p����Rootsignature���쐬
	*	@param[in]	dev		ID3D12Device�̎Q��
	*/
	void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	@brief	PMD���f���𑀍삷��N���X�̍쐬
	*	@param[in]	model	�R���g���[�����郂�f��
	*	@param[in]	path	���f���̃��[�h�Ƃ��Ɏw�肳�ꂽ�t�@�C���p�X
	*	@return	PMD���f���𑀍삷��R���g���[���[
	*/
	std::shared_ptr<PMDController> CreateController(std::shared_ptr<PMDModel>& model, const std::string& path);

	/**
	*	@brief	�w�肳�ꂽ�p�X�̃��f���̖��O�𓾂�
	*	@param[in]	path	���f���̖��O���~�������f���ւ̃t�@�C���p�X
	*	@return	���f���̖��O
	*/
	std::string GetModelName(const std::string& path) const;

	void CreateRigidBodis();
	
};