#pragma once
#include "DirectX12/DrawObject/DrawController3D.h"
#include "FbxStructure.h"

#include <vector>
#include <memory>
#include <DirectXMath.h>

class ConstantBufferObject;
class VertexBufferObject;
class IndexBufferObject;
class LightObject;
class FbxModelDataConverter;
class RootSignatureObject;
class PipelineStateObject;
class FbxAnimationPlayer;
class FbxAnimationData;
class Dx12DescriptorHeapObject;
class Dx12Camera;
class Dx12CommandList;

namespace Fbx
{
	class FbxModel;
}

class FbxModelController :
	public DrawController3D
{
	friend FbxModelDataConverter;
public:
	FbxModelController(std::shared_ptr<Fbx::FbxModel>& model, 
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		std::shared_ptr<Dx12CommandList>& cmdList,
		std::shared_ptr<PipelineStateObject>& pipelinestate,
		std::shared_ptr<RootSignatureObject>& rootsignature);
	~FbxModelController();

	void Draw();
	void DrawSkeleton();
	void SetLight(std::shared_ptr<LightObject> dirlight);
	void SetGraphicsRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);
	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);
	void SetAnimation(std::shared_ptr<FbxAnimationData>& animation, bool isLoop = true);
	void StopAnimation() const;
	void RestartAnimation() const;
	bool IsAnimationEnd() const;
	void AddConstantBuffer(std::shared_ptr<ConstantBufferObject>& buffer);
	std::string GetModelName() const;
	std::string GetModelPath() const;

	void UpdateDescriptorHeap();
private:
	std::shared_ptr<FbxAnimationPlayer> mAnimationPlayer;

	std::vector<Fbx::FbxVertexElement> mVertexElements;
	std::shared_ptr<VertexBufferObject> mCtrlVertexBuffer;
	std::shared_ptr<Fbx::FbxModel> mModel;

	std::shared_ptr<ConstantBufferObject> mDirLightBuffer;

	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;
	std::vector<std::shared_ptr<ConstantBufferObject>> mAddConstantBuffers;

	void (FbxModelController::*mBundleUpdate)();
	void (FbxModelController::*mSkeletonDraw)();

	void UpdateVertex();
	void UpdateBundle();
	void NonBundleUpdate();
	void NonDrawSkeleton();
	void DrawColorSkeleton();
};
