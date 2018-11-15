#pragma once
/**
*	@addtogroup Dx12RootSignature
*	@file RootSignatureObject.h
*	@brief ID3D12RootSignature���쐬���ێ�����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/18
*/
#include "Shader/ShaderDatasStructure.h"

#include <vector>
#include <d3dcommon.h>
#include <wrl.h>
#include <d3d12.h>

struct ID3D12RootSignature;
struct ID3D10Blob;

/**
*	@ingroup Dx12RootSignature
*	@class RootSignatureObject
*	@brief ID3D12RootSignature���쐬���ێ�����
*/
class RootSignatureObject
{
public:
	/**
	*	@param[in]	signatureBlob	rootsiganture���`���Ă����������blob
	*	@param[in]	dev		ID3D12Device�̎Q��
	*/
	RootSignatureObject(const std::string& name,ID3D10Blob* signatureBlob, Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	virtual ~RootSignatureObject();

	/**
	*	@brief	�g�p����V�F�[�_�[��ݒ肷��
	*	@param[in]	shader	�ݒ肷��V�F�[�_�[
	*/
	void SetShaderData(const ShaderDatas& shader);

	/**
	*	@brief	�g�p���Ă���V�F�[�_�[���擾����
	*	@return	�g�p���Ă���V�F�[�_�[
	*/
	ShaderDatas& GetShaderDatas();

	/**
	*	@brief	ID3D12RootSignature���擾����
	*	@return	�쐬����rootsignature
	*/
	Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetRootSignature();
protected:
	RootSignatureObject();

	/**
	*	@brief	rootsignature���쐬����
	*	@param[in]	signatureBlob	rootsignature�̏�������blob
	*	@param[in]	dev		ID3D12Device�̎Q��
	*/
	void CreateRootSignature(const std::string& name, ID3D10Blob* signatureBlob, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	�쐬����rootsigantrue
	*/
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;

	/**
	*	�g�p����V�F�[�_�[
	*/
	ShaderDatas mShader;
private:
};

