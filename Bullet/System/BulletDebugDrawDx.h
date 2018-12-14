#pragma once
/**
*	@file	BulletDebugDrawDx.h
*	@brief	bulletのデバッグ描画用クラス定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/26
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
*	@brief デバッグ描画を定義する
*/
class BulletDebugDrawDx : public btIDebugDraw
{
public:
	BulletDebugDrawDx(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~BulletDebugDrawDx();

	/**
	*	@brief	線を引く 主にバレット側が呼ぶ関数なので直接呼ぶ必要はない
	*	@param[in]	from	起点
	*	@param[in]	to		終点
	*	@param[in]	color	描画色
	*/
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	/**
	*	@brief	デバッグ描画モードを設定する
	*	@param	デバッグモード
	*/
	void setDebugMode(int debugMode);

	/**
	*	@brief	現在のデバッグモードを取得する
	*	@return	現在のデバッグモード
	*/
	int	getDebugMode() const;

	/**
	*	未実装　何もしない
	*/
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	/**
	*	未実装 何もしない
	*/
	void reportErrorWarning(const char* warningString);

	/**
	*	未実装　何もしない
	*/
	void draw3dText(const btVector3& location, const char* textString);

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
	std::shared_ptr<PipelineStateObject> mPipelinestate;
	std::shared_ptr<RootSignatureObject> mRootsignature;
	std::shared_ptr<VertexBufferObject> mVertexBuffer;
	std::shared_ptr<ConstantBufferObject> mCamerasBuffer;
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;
	Microsoft::WRL::ComPtr<ID3D12Device> mDev;
};

