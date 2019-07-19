#pragma once
/**
*	@addtogroup primitive
*	@file	PrimitiveController.h
*	@brief	primitive�N���X�Œ�`�������_��`�悷��N���X��`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/27
*/
#include "DirectX12/DrawObject/DrawController3D.h"
#include <vector>

class PrimitiveObject;
class VertexBufferObject;
class ConstantBufferObject;
class Dx12DescriptorHeapObject;
class IndexBufferObject;
class LightObject;
class Dx12Camera;
class TextureObject;
class Dx12BufferObject;
class Dx12CommandList;
struct PrimitiveVertex;

/**
*	@ingroup primitive
*	@class PrimitiveController
*	@brief primitive�N���X�Œ�`�������_��`�悷��N���X
*/
class PrimitiveController :
	public DrawController3D
{
public:
	/**
	*	@param[in]	primitive	���_���쐬�����v���~�e�B�u�N���X
	*	@param[in]	dev			�g�p����ID3D12Device
	*	@param[in]	cmdList		�g�p����R�}���h���X�g
	*/
	PrimitiveController(std::shared_ptr<PrimitiveObject> primitive
		, Microsoft::WRL::ComPtr<ID3D12Device>& dev
		, std::shared_ptr<Dx12CommandList>& cmdList);

	PrimitiveController(std::vector<PrimitiveVertex>& vertices, std::vector<unsigned int> indices
		, Microsoft::WRL::ComPtr<ID3D12Device>& dev
		, std::shared_ptr<Dx12CommandList>& cmdList);

	virtual ~PrimitiveController();

	/**
	*	@brief	�e�N�X�`����ݒ肷��
	*	@param[in]	texture		�ݒ肷��e�N�X�`��
	*/
	void SetTexture(std::shared_ptr<TextureObject>& texture);

	/**
	*	@brief	���C�g��ݒ肷��
	*	@param[in]	light	�ݒ肷�郉�C�g
	*/
	void SetLightBuffer(std::shared_ptr<LightObject>& light);

	/**
	*	@brief	�C���X�^���V���O�ŃI�u�W�F�N�g��`�悷��
	*	@param[in]	instancePositions	�e�I�u�W�F�N�g�̈ʒu
	*/
	void Instancing(std::vector<DirectX::XMFLOAT3>& instancePositions);

	/**
	*	@brief	�C���X�^���V���O�`�掞�Ɏg���A�t�B���s���ݒ肷��
	*	@param[in]	matrix	�A�t�B���s��
	*	@param[in]	startIndex	�s���K������I�u�W�F�N�g�̎n�߂̔ԍ�
	*	@param[in]	endIndex	�s���K������I�u�W�F�N�g�̍Ō�̔ԍ�
	*/
	void SetInstancingMatrix(std::vector<DirectX::XMFLOAT4X4>& matrix, unsigned int startIndex, unsigned int endIndex);

	/**
	*	@brief	�A�t�B���s����擾����
	*	@return		�A�t�B���s��
	*/
	DirectX::XMFLOAT4X4 GetMatrix() const;

	/**
	*	�`�悷��
	*/
	void Draw();

	/**
	*	@brief	���W��ݒ肷��
	*	@param[in]	pos		�ݒ肷����W
	*/
	void SetPosition(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	�g�嗦�𓙔{�Őݒ肷��
	*	@param[in]	scale	�g�嗦
	*/
	void SetScale(float scale);

	/**
	*	@brief	�g�嗦���e��ݒ肷��
	*	@param[in]	scaleX	x�̊g�嗦
	*	@param[in]	scaleY	y�̊g�嗦
	*	@param[in]	scaleZ	z�̊g�嗦
	*/
	void SetScale(float scaleX, float scaleY, float scaleZ);

	/**
	*	@brief	x�����S�Œǉ��ŉ�]������
	*	@param[in]	rad		��]��(radian,�ʓx�@)
	*/
	void AddRotaX(float rad);

	/**
	*	@brief	y�����S�Œǉ��ŉ�]������
	*	@param[in]	rad		��]��(radian,�ʓx�@)
	*/
	void AddRotaY(float rad);

	/**
	*	@brief	z�����S�Œǉ��ŉ�]������
	*	@param[in]	rad		��]��(radian,�ʓx�@)
	*/
	void AddRotaZ(float rad);

	/**
	*	@brief	�C�ӂ̎��ŉ�]����l������ݒ肷��
	*	@param[in]	quaternion	���Ɖ�]�ʂ�\���l����
	*/
	void SetRotaQuaternion(const DirectX::XMFLOAT4& quaternion);

	/**
	*	DescriptorHeap���X�V����
	*	��{�I�ɂ͌Ă΂Ȃ��Ă���
	*/
	void UpdateDescriptorHeap();

	/**
	*	@brief �v���~�e�B�u�̐F��ݒ肷��
	*	@param color 0~1�͈̔͂�rbg�̐F���
	*/
	void SetColor(const DirectX::XMFLOAT4& color, int index = 0);

	void SetShadowmapRootSignature(std::shared_ptr<RootSignatureObject> rootsignature);

	void SetShadowmapPipelineState(std::shared_ptr<PipelineStateObject> pipelinestate);

	void SetShadowRenderRootSignature(std::shared_ptr<RootSignatureObject> rootsignature);

	void SetShadowRenderPipelineState(std::shared_ptr<PipelineStateObject> pipelinestate);

	void SetShadowmapTexture(std::shared_ptr<Dx12BufferObject> shadowmapTex);

	void CreateShadowmapDescHeap();

	void DrawShadowmap();

	void DrawShadow();

	void SetShadowmapCommandList(std::shared_ptr<Dx12CommandList>& cmdList);

	void SetGraphicsRootSignature(const std::shared_ptr<RootSignatureObject>& rootSignature);

	void SetPipelineState(const std::shared_ptr<PipelineStateObject>& pipelineState);

protected:
	void UpdateInstanceVertexBuffer();
	void NonUpdate();
	void Initialize(Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE,
		eROOT_PARAMATER_INDEX_CAMERA,
		eROOT_PARAMATER_INDEX_LIGHT,
		eROOT_PARAMATER_INDEX_SHADOWMAP,
		eROOT_PARAMATER_INDEX_MAX
	};

	struct InstanceDatas
	{
		DirectX::XMFLOAT4X4 aMatrix;
		DirectX::XMFLOAT4 offset;
		DirectX::XMFLOAT4 color;
	};
	std::shared_ptr<PrimitiveObject> mPrimitive;
	std::shared_ptr<VertexBufferObject> mVertexBuffer;
	std::shared_ptr<IndexBufferObject> mIndexBuffer;
	std::vector<InstanceDatas> mInstanceDatas;
	std::shared_ptr<VertexBufferObject> mInstanceVertexBuffer;
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;
	std::shared_ptr<ConstantBufferObject> mLightBuffer;
	std::shared_ptr<TextureObject> mTexObj;
	void (PrimitiveController::*mInstanceUpdate)();
	void (PrimitiveController::*mDescHeapUpdate)();
	std::shared_ptr<PipelineStateObject> mShadowmapPipeline;
	std::shared_ptr<PipelineStateObject> mShadowRenderPipeline;
	std::shared_ptr<RootSignatureObject> mShadowmapRootSignature;
	std::shared_ptr<RootSignatureObject> mShadowRenderRootSignature;
	std::shared_ptr<Dx12BufferObject> mShadowmapTexture;
	std::shared_ptr<Dx12CommandList> mShadowmapCmdList;
};

