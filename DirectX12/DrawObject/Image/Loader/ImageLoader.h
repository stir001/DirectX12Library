#pragma once
/**
*	@file ImageLoader.h
*	@brief 2D�摜�����[�h�������ێ��A�Ǘ�����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/12/19
*/

#include "DirectX12/Shader/ShaderDatasStructure.h"
#include "DirectX12/DrawObject/DrawObjectLoader.h"

#include <string>
#include <map>
#include <memory>
#include <d3d12.h>

class ImageController;
class Image3DController;
class ImageObject;
class RootSignatureObject;
class PipelineStateObject;

/**
*	@ingroup DrawObjectLoader
*	@class ImageLoader
*	@brief 2D�摜�����[�h���Ă����ێ��A�Ǘ�����
*			�ێ����Ă�����̂�������x���[�h���悤�Ƃ����ꍇ���łɃ��[�h���Ă�����̂��g���ď���������
*			�f�[�^��������邽�߂ɂ�Release�֐����Ă�
*/
class ImageLoader : public DrawObjectLoader
{
public:
	~ImageLoader();

	/**
	*	@brief	2D�摜�����[�h���A2D�Ƃ��đ��삷��R���g���[���[��Ԃ�
	*	@param[in]	path	���[�h����2D�摜�p�X
	*/
	std::shared_ptr<ImageController> LoadImageData(const std::string& path);

	/**
	*	@brief	2D�摜�����[�h���A3D�Ƃ��đ��삷��R���g���[���[��Ԃ�
	*	@param[in]	path	���[�h����2D�摜�p�X
	*/
	std::shared_ptr<Image3DController> LoadImage3D(const std::string& path, bool isBillboard = false);
	
	/**
	*	@brief UI���C���[�̃R�}���h���X�g��ݒ肷��
	*	@param[in]	cmdList	�ݒ肷��R�}���h���X�g
	*/
	void SetUIPassCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);

	/**
	*	@brief BackGround���C���[�̃R�}���h���X�g��ݒ肷��
	*	@param[in]	cmdList	�ݒ肷��R�}���h���X�g
	*/
	void SetBackGroundPassCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);

	/**
	*	@brief Model���C���[�̃R�}���h���X�g��ݒ肷��
	*	@param[in]	cmdList	�ݒ肷��R�}���h���X�g
	*/
	void Set3DPassCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);

	/**
	*	@brief	�w�肳�ꂽ�t�@�C���p�X�Ń��[�h�����I�u�W�F�N�g�̏��L���������
	*	@param[in]	releaseImagePath	����������t�@�C���p�X
	*/
	void Release(const std::string& releaseImagePath);

	/**
	*	@brief	ImageLodaer�̎Q�Ƃ��擾����
	*	@return	ImageLoader�̎Q��
	*/
	static  ImageLoader& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new ImageLoader();
		}
		return *mInstance;
	}

	/**
	*	@brief	ImageLoader��j������
	*/
	static void Destroy()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}
private:
	ImageLoader();
	ImageLoader(const ImageLoader&);
	ImageLoader(const ImageLoader&&);
	ImageLoader& operator=(const ImageLoader&);

	/**
	*	�C���X�^���X�̎��ԕێ��p
	*/
	static ImageLoader* mInstance;

	/**
	*	2D�摜���Ǘ��p
	*/
	std::map<std::string, std::shared_ptr<ImageObject>> mImages;

	/**
	*	�w�i���C���[�p�̃R�}���h���X�g
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mBackCmdList;

	/**
	*	3D�`�惌�C���[�p�̃R�}���h���X�g
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mModelCmdList;
	
	/**
	*	2D�摜�`��Ɏg�p����rootsiganture
	*/
	std::shared_ptr<RootSignatureObject> mRootsignature;

	/**
	*	2D�摜�`��Ɏg�p����pipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mPipelinestate;

	/**
	*	2D�摜��3D�Ƃ��ĕ`�悷��Ƃ��Ɏg�p����rootsiganaure
	*/
	std::shared_ptr<RootSignatureObject> m3DRootsignature;

	/**
	*	2D�摜��3D�Ƃ��ĕ`�悷��Ƃ��Ɏg�p����rootsignature
	*/
	std::shared_ptr<PipelineStateObject> m3DPipelinestate;

	/**
	*	2D�摜��3D�̃r���{�[�h�Ƃ��ĕ`�悷��Ƃ��Ɏg�p����rootsignature
	*/
	std::shared_ptr<RootSignatureObject> mBillboardRootsignature;

	/**
	*	2D�摜��3D�̃r���{�[�h�Ƃ��ĕ`�悷��Ƃ��Ɏg�p����pipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mBillboardPipelineState;

	/**
	*	2D�摜�`��p�̃V�F�[�_�[���
	*/
	ShaderDatas mShader;

	/**
	*	2D�摜��3D�Ƃ��ĕ`�悷��Ƃ��̃V�F�[�_���
	*/
	ShaderDatas m3DShader;

	/**
	*	@brief	pipelinestate���쐬����
	*	@param[in]	dev		ID3D12Device�̎Q��
	*/
	void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	@brief	rootsignature���쐬����
	*	@param[in]	dev		ID3D12Device�̎Q��
	*/
	void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
};

