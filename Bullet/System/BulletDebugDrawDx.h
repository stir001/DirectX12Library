#pragma once
/**
*	@file	BulletDebugDrawDx.h
*	@brief	bullet�̃f�o�b�O�`��p�N���X��`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/26
*/
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <vector>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d12.h>

class PipelineStateObject;
class RootSignatureObject;
class VertexBufferObject;
class IndexBufferObject;
class ConstantBufferObject;
class Dx12DescriptorHeapObject;

/**
*	@class BulletDebugDrawDx
*	@brief �f�o�b�O�`����`����
*/
class BulletDebugDrawDx : public btIDebugDraw
{
public:
	BulletDebugDrawDx(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~BulletDebugDrawDx();

	/**
	*	@brief	�������� ��Ƀo���b�g�����ĂԊ֐��Ȃ̂Œ��ڌĂԕK�v�͂Ȃ�
	*	@param[in]	from	�N�_
	*	@param[in]	to		�I�_
	*	@param[in]	color	�`��F
	*/
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	/**
	*	@brief	�f�o�b�O�`�惂�[�h��ݒ肷��
	*	@param	�f�o�b�O���[�h
	*/
	void setDebugMode(int debugMode);

	/**
	*	@brief	���݂̃f�o�b�O���[�h���擾����
	*	@return	���݂̃f�o�b�O���[�h
	*/
	int	getDebugMode() const;

	/**
	*	�������@�������Ȃ�
	*/
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	/**
	*	������ �������Ȃ�
	*/
	void reportErrorWarning(const char* warningString);

	/**
	*	�������@�������Ȃ�
	*/
	void draw3dText(const btVector3& location, const char* textString);

	void ClearLines();

	void DebugDraw();
private:
	int mMode;
	struct DebugDrawVertex
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 color;
		DebugDrawVertex() : position(), color() {}
		DebugDrawVertex(const btVector3& pos, const btVector3& color) 
			: position(static_cast<float>(pos.x()), static_cast<float>(pos.y()), static_cast<float>(pos.z()) , 1.0f)
			, color(static_cast<float>(color.x()), static_cast<float>(color.y()), static_cast<float>(color.z()), 1.0f) {}
	};

	std::vector<DebugDrawVertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::shared_ptr<PipelineStateObject> mPipelinestate;
	std::shared_ptr<RootSignatureObject> mRootsignature;
	std::shared_ptr<VertexBufferObject> mVertexBuffer;
	std::shared_ptr<IndexBufferObject> mIndexBuffer;
	std::shared_ptr<ConstantBufferObject> mCamerasBuffer;
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;
	Microsoft::WRL::ComPtr<ID3D12Device> mDev;
};

