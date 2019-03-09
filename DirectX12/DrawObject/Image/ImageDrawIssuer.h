#pragma once
#include "DrawObject\DrawCallIssuer.h"
#include <string>
#include <vector>
#include <wrl.h>

class Dx12DescriptorHeapObject;
struct ID3D12Device;

class ImageDrawIssuer :
	public DrawCallIssuer
{
public:
	/**
	*	�R���X�g���N�^����
	*/
	struct ConstructorArg
	{
		std::string name;
		std::shared_ptr<Dx12DescriptorHeapObject> descHeap;
		unsigned int texViewIndex;
		std::shared_ptr<VertexBufferObject> vertexBuffer;
		std::shared_ptr<VertexBufferObject> uvBuffer;
		std::shared_ptr<VertexBufferObject> matrixBuffer;
		std::shared_ptr<PipelineStateObject> pipelineState;
		std::shared_ptr<RootSignatureObject> rootSignature;
		Microsoft::WRL::ComPtr<ID3D12Device> dev;
	};

	ImageDrawIssuer(const ImageDrawIssuer::ConstructorArg& arg);
	~ImageDrawIssuer();

	/**
	*	@brief	�`�施�߂𔭍s����
	*	@param[in]	cmdList	���߂�ςރR�}���h���X�g
	*/
	void IssueDrawCall(std::shared_ptr<Dx12CommandList>& cmdList);

	/**
	*	@brief	�o���h�����X�V����
	*/
	void UpdateBundle();

	/**
	*	@brief	pipelineState��ݒ肷��
	*	@param[in]	pipelineState	�g�p����PipelineState
	*/
	void SetPipelineState(const std::shared_ptr<PipelineStateObject>& pipelineState);

	/**
	*	@brief	pipelineState��ݒ肷��
	*	@param[in]	pipelineState	�g�p����PipelineState
	*/
	void SetRootSignature(const std::shared_ptr<RootSignatureObject>& rootSignature);

	/**
	*	@brief	��{�̒��_���������_�o�b�t�@��ݒ肷��
	*/
	void SetVertexBuffer(const std::shared_ptr<VertexBufferObject>& vertex);

	/**
	*	@brief	UV���������_�o�b�t�@��ݒ肷��
	*/
	void SetUVBuffer(const std::shared_ptr<VertexBufferObject>& uv);

	/**
	*	@brief	��ԕω��p�s��������_�o�b�t�@��ݒ肷��
	*/
	void SetMatrixBuffer(const std::shared_ptr<VertexBufferObject>& mat);
private:
	/**
	*	����rootsiganture�Ŏg�����[�g�p�����[�^�[
	*/
	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE,
	};

	enum eVertexBufferID
	{
		VERTEX,
		UV,
		MATRIX,
		VERTEXIDR_MAX,
	};

	/**
	*	�ʏ�`�掞�Ɏg�p����rootsignature
	*/
	std::shared_ptr<RootSignatureObject> mRootsignature;

	/**
	*	�ʏ�`�掞�Ɏg�p����pipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mPipelinestate;

	/**
	*	�q�[�v
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;

	/**
	*	�e�N�X�`����DescriptorHrap��View�ԍ�
	*/
	unsigned int mTexViewIndex;

	/**
	*	�o�[�e�b�N�X�o�b�t�@�[
	*/
	std::vector<std::shared_ptr<VertexBufferObject>> mVertexBuffers;

};

